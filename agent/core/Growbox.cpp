

#include <algorithm>
#include <csignal>
#include <iostream>
#include <iomanip>
#include <pwd.h>
#include <sqlite3.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <uv.h>
#include <unordered_map>

using namespace std;

#include "Base.hpp"
#include "Growbox.hpp" 
#include "GrowboxManager.hpp" 
#include "Recorder.hpp"

void TimelinesMainCallBack(uv_timer_t* handle);

void TimelinesMainCallBack(uv_timer_t* handle)
{
    void *data = uv_handle_get_data((uv_handle_t *)handle);
    twogreencows_core::Growbox *tmpGrowbox = static_cast<twogreencows_core::Growbox *>(data);
    cout << "================= RECEIVING FOR : " << tmpGrowbox->GetIdentifier(true) << endl;
    tmpGrowbox->HandleTimerCallback();
}



namespace twogreencows_core
{

    string Growbox::GetClassPrefix() const
    {
        return "growbox";
    }

    int Growbox::GetClassVersion() const
    {
        return 1;
    }


    Growbox::Growbox(unordered_map<std::string, std::string> parameters)
    {
        std::string identifier= "";
        if (parameters.find("identifier") != parameters.end()) {
            identifier = parameters.at("identifier");   
        } 
        this->SetUpIdentifier(identifier);
        this->ObjectVersion = 1;        

        if (parameters.find("name") == parameters.end()) {
            this->name = "growbox generic";
        } else {
            this->name = parameters.at("name");
        }

        if (parameters.find("server_ipv4") == parameters.end()) {
            this->serverIPV4 = "0.0.0.0";
        } else {
            this->serverIPV4 = parameters.at("server_ipv4");
        }
     
        this->timelines = new vector<Timeline *>();
        this->recorders= new vector<Recorder *>();
        this->device = new Device();
        this->dataFolderPath = GrowboxManager::GetSharedManager()->GetTopFolderPathForGrowboxIdentifier(this->GetIdentifier());             //Set up signals
        signal(SIGINT, Growbox::signalHandler); 
        signal(SIGILL, Growbox::signalHandler); 
        signal(SIGTERM, Growbox::signalHandler); 

      
             //Create the recorders and open them
        Recorder *LogRecorder = Recorder::CreateRecorder(Recorder::LOG, this->GetIdentifier());
        this->recorders->push_back(LogRecorder);
        Recorder *NetworkRecorder = Recorder::CreateRecorder(Recorder::NETWORK, this->GetIdentifier());
        this->recorders->push_back(NetworkRecorder);
        for(std::vector<Recorder*>::iterator itt = this->recorders->begin(); itt != this->recorders->end(); ++itt) {
            (*itt)->Open();
        }

        cerr << "== CREATE GrowBox: " + this->GetIdentifier() + " (" + this->GetName() + ")"<< endl;
        unordered_map<DataPoint::DataPointKey, std::any> data = unordered_map<DataPoint::DataPointKey, std::any>({
                {DataPoint::SUBJECT_NAME, this->GetName()}
                });
        DataPoint dp = DataPoint(DataPoint::GROWBOX_CREATE, this->GetIdentifier(), data);
        this->LogDataPoint(dp);
        
    }

    void Growbox::signalHandler(int signum)
    {
        exit(0);
    }

    string Growbox::GetName()
    {
        return(this->name);
    }

    string Growbox::GetDataFolderPath()
    {
        return this->dataFolderPath;
    }

    void Growbox::AddTimeline(Timeline *tl)
    {
        timelines->push_back(tl);
        tl->SetOwnerIdentifier(this->GetIdentifier());
    }

    void Growbox::Start()
    {
        std::time(&start_date);
        stop_date = 0;
        isRunning = true;


        struct tm * timeinfo;
        timeinfo = std::localtime(&start_date);
        long SecondsInDay = timeinfo->tm_hour * 3600 + timeinfo->tm_min *60 + timeinfo->tm_sec;

        cout << "== START Growbox: " << this->name << " ("<< this->GetIdentifier(true) <<")" << ctime(&start_date);
        long start = SecondsInDay;
        long start_m_s = start % 3600;
        long start_h = (start - start_m_s) /3600;
        long start_s = start_m_s % 60;
        long start_m =  (start_m_s - start_s) / 60;



        unordered_map<DataPoint::DataPointKey, std::any> data = unordered_map<DataPoint::DataPointKey, std::any>();
        DataPoint dp = DataPoint(DataPoint::GROWBOX_START, this->GetIdentifier(), data);
        this->LogDataPoint(dp);
        
        cout << "> Time is at " << std::dec <<SecondsInDay << " in day " 
            <<  std::setfill('0') << std::setw(2)<< start_h <<"H:"
            << std::setfill('0') << std::setw(2)<< start_m << "m:" 
            <<std::setfill('0') << std::setw(2)<< start_s << "s " <<endl;

        this->nextEvents = new vector<Event*>();

        //cout << "= Start Timeline triggers " << endl;

        for(std::vector<Timeline *>::iterator itt = this->timelines->begin(); itt != this->timelines->end(); ++itt) {
            std::vector<Event *> triggerEvents =  (*itt)->StartTriggers(0, SecondsInDay);
            for (std::vector<Event *>::iterator ite = triggerEvents.begin(); ite != triggerEvents.end(); ite++) {
                this->nextEvents->push_back(*ite);
            }
        }

        std::sort(this->nextEvents->begin(), this->nextEvents->end(), [](Event* a, Event * b) {
                return a->GetRelativeSecondsToNext() < b->GetRelativeSecondsToNext(); });

        cout << "== INFO Dumping next events " << endl;
        for(std::vector<Event *>::iterator it = this->nextEvents->begin(); it != this->nextEvents->end(); ++it) {
            cout << "> " << *(*it) <<endl;
        }

        this->waitsForEvent = true;
        this->waitTime =  nextEvents->at(0)->GetRelativeSecondsToNext();

        cout << "== INFO Will wake up in " << this->waitTime << endl;

        this->loop = uv_default_loop();
        uv_timer_init(loop, &(this->timer_req));
        uv_handle_set_data((uv_handle_t *) &(this->timer_req), static_cast<void *>(this));
        uv_timer_start(&(this->timer_req), TimelinesMainCallBack, this->waitTime*1000, 0);
        GrowboxManager::GetSharedManager()->SetLastActiveGrowboxIdentifier(this->GetIdentifier());
        uv_run(this->loop, UV_RUN_DEFAULT);        

        delete  this->nextEvents;

    }

    void Growbox::HandleTimerCallback()
    {

        time_t currentTime = 0;
        struct tm *currentTimeInfo;
        std::time(&currentTime);   
        currentTimeInfo = std::localtime(&currentTime);
        long SecondsInDay = currentTimeInfo->tm_hour * 3600 + currentTimeInfo->tm_min *60 + currentTimeInfo->tm_sec;

        cout << "== INFO Received timer " << " at " << SecondsInDay <<endl;
        if (true == this->waitsForEvent ) {

            cout << "== INFO Fire ot triggers " <<  endl;
            //trigger the Fire

            for(std::vector<Event *>::iterator it = nextEvents->begin(); it != nextEvents->end(); ++it) {
                if ((*it)->GetRelativeSecondsToNext() == this->waitTime) {
                    Trigger *tr = static_cast<Trigger *>(Base::ObjectWithIdentifier((*it)->GetTriggerIdentifier()));
                    cout << "== INFO      " << tr->GetName() << endl;
                    tr->Fire(*it, SecondsInDay);
                } 
                (*it)->SetRelativeSecondsToNext(0);
            }
            //wait for one second to avoid looping on the action
            this->waitsForEvent = false;
            //cout << " == Schedule for 2 second " <<  endl;
            uv_timer_stop(&(this->timer_req));
            uv_timer_start(&(this->timer_req), TimelinesMainCallBack, 2000, 0);

        }  else {

            //cout << " == Schedule next events" <<  endl;
            this->waitsForEvent = true ;

            this->nextEvents->clear();

            for(std::vector<Timeline *>::iterator itt = this->timelines->begin(); itt != this->timelines->end(); ++itt) {
                std::vector<Event *> triggerEvents =  (*itt)->GetNextEventForTriggers(0, SecondsInDay);
                for (std::vector<Event *>::iterator ite = triggerEvents.begin(); ite != triggerEvents.end(); ite++) {
                    nextEvents->push_back(*ite);
                }
            }


            std::sort(nextEvents->begin(), nextEvents->end(), [](Event* a, Event * b) {
                    return a->GetRelativeSecondsToNext() < b->GetRelativeSecondsToNext(); });
            cout << "== INFO Dumping next events " << endl;
            for(std::vector<Event *>::iterator it = nextEvents->begin(); it != nextEvents->end(); ++it) {
                cout << "> " << *(*it) <<endl;
            }


            this->waitTime = nextEvents->at(0)->GetRelativeSecondsToNext();

            cout << " == Will wake up in " << this->waitTime << endl;
            uv_timer_stop(&(this->timer_req));
            uv_timer_start(&(this->timer_req), TimelinesMainCallBack, this->waitTime*1000, 0);
        }

    }
    void Growbox::Stop()
    {

        this->stop_date = std::time(0);

        uv_timer_stop(&(this->timer_req));
        for(std::vector<Recorder*>::iterator itt = this->recorders->begin(); itt != this->recorders->end(); ++itt) {
            (*itt)->Close();
        }

        unordered_map<DataPoint::DataPointKey, std::any> data = unordered_map<DataPoint::DataPointKey, std::any>();
        DataPoint dp = DataPoint(DataPoint::GROWBOX_STOP, this->GetIdentifier(), data);
    }

    void Growbox::LogDataPoint(DataPoint dp)
    {
        for(std::vector<Recorder*>::iterator itt = this->recorders->begin(); itt != this->recorders->end(); ++itt) {
            (*itt)->LogDataPoint(dp);
        }
    }

    bool Growbox::PersistToStorage(sqlite3 *db)
    {   
        sqlite3_stmt *res;
        bool result = false;
        cout << "persist " << this->GetIdentifier() <<endl;
            //Check presence
        string sql = "SELECT * FROM growboxes where uuid = @uuid;";
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
        if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}

        int idx = sqlite3_bind_parameter_index(res, "@uuid");        
        sqlite3_bind_text(res, idx, this->GetIdentifier().c_str(), -1, SQLITE_STATIC);
    
        int step = sqlite3_step(res);
        sqlite3_finalize(res);

        if (step == SQLITE_ROW) {

            sql = "UPDATE growboxes set (name, server_ipv4, object_version, class_version) values (@name, @server_ipv4, @object_version, @class_version);";
              cout <<  sqlite3_column_text(res, 0) << endl;
              cout << sqlite3_column_text(res, 1) << endl;
        } else {
            sql = "INSERT INTO growboxes (uuid, name, server_ipv4, object_version, class_version) values ( @uuid, @name, @server_ipv4, @object_version, @class_version);";
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0); 
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            int idx = sqlite3_bind_parameter_index(res, "@uuid");        
            

            rc = sqlite3_bind_text(res, idx,this->GetIdentifier().c_str() , -1, SQLITE_TRANSIENT);
            cout << rc <<endl;
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}

            idx = sqlite3_bind_parameter_index(res, "@name");        
            rc = sqlite3_bind_text(res, idx, this->GetName().c_str(), -1, SQLITE_STATIC);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            idx = sqlite3_bind_parameter_index(res, "@server_ipv4");    
            rc = sqlite3_bind_text(res, idx, this->serverIPV4.c_str(), -1, SQLITE_STATIC);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement: " <<  sqlite3_errmsg(db) << endl; return result;}
            
            idx = sqlite3_bind_parameter_index(res, "@object_version");        
            rc = sqlite3_bind_int(res, idx, this->ObjectVersion);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            idx = sqlite3_bind_parameter_index(res, "@class_version");        
            rc = sqlite3_bind_int(res, idx, this->GetClassVersion());
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            rc = sqlite3_step(res);
            if (rc != SQLITE_OK && rc !=SQLITE_DONE)  { cerr <<  "Failed to execute statement:" <<  sqlite3_errmsg(db) << endl; return result;}
            sqlite3_finalize(res);
        } 
        result = true;
        return result;
    }
}

//std::ostream &operator<<(std::ostream &os, twogreencows_core:: GrowBox const &e) { 
//    return os << e;
//}
