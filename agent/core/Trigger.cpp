#include <climits>
#include <string>
#include <iostream>
#include "Trigger.hpp"
#include "Base.hpp"
#include "Growbox.hpp"
#include "DataPoint.hpp"
#include "Action.hpp"

using namespace std;


namespace twogreencows_core
{

    string Trigger::GetClassPrefix() const
    {
        return "trigger";
    }

    int Trigger::GetClassVersion() const
    {
        return 1;
    }


    Trigger::Trigger(string name, Action *action , vector<pair<time_t, Base::State> > sequence , bool transient, std::string identifier)
    {
        this->SetUpIdentifier(identifier);
        this->ObjectVersion = 1; 
        this->name = name;
        this->transient = transient;
        this->sequence = new vector<Event* >();
        this->action = action;

        cerr << "== CREATE     Trigger: " << this->GetIdentifier() + "(" + this->GetName() + ")"<< endl;
        //validate sequence: it has to cover 24 hours in case of
        long tmpLong = 0;
        bool hasOFFState = false;
        for(std::vector<pair<time_t, Base::State> >::iterator it = sequence.begin(); it != sequence.end(); ++it) {
            tmpLong += (*it).first;
            if (!hasOFFState) {
                hasOFFState = ((*it).second == Base::OFF);
            }
        }
        if (this->transient == false) {
            if (tmpLong != 86400) {
                Base::DumpSequence(sequence, transient);
                throw std::invalid_argument("Sequence for non transient trigger must cover 24 hours");
            }
        } else {
            if (hasOFFState) {
                cerr << "== WARNING     For transient trigger, OFF part will be ignored. A sequence should be a suite of ON state" << endl;
            }
        }

        //fill the sequence with incremental date
        tmpLong = 0;
        for(std::vector<pair<time_t, Base::State> >::iterator it = sequence.begin(); it != sequence.end(); ++it) {
            Event *tmpEvent = new Event(tmpLong, (*it).first, this->GetIdentifier(), this->GetOwnerIdentifier(), (*it).second);
            this->sequence->push_back(tmpEvent);
            tmpLong += (*it).first;
        }

    }


    Trigger::~Trigger()
    {
        delete(this->sequence);
    }

    string Trigger::GetName()
    {
        return(this->name);
    }

    bool Trigger::GetTransient() 
    {
        return(this->transient);
    }

    void Trigger::SetTransient(bool newValue)
    {
        this->transient = newValue;
    }

    Base::State Trigger::GetState() const
    {
        return this->state;
    }

    void Trigger::SetState(Base::State newState)
    {
        this->state = newState;
        string tmpValue = (newState == Base::ON) ? "ON" : "OFF";

        if (this->action) {
            unordered_map<DataPoint::DataPointKey, std::any> data = this->action->Execute(this->GetIdentifier());
            DataPoint dp = DataPoint(DataPoint::TRIGGER_DATAEVENT, this->GetIdentifier(), data);
            Timeline *timeline = static_cast<twogreencows_core::Timeline*>(Base::ObjectWithIdentifier(this->GetOwnerIdentifier()));
            Growbox *growbox = static_cast<twogreencows_core::Growbox*>(Base::ObjectWithIdentifier(timeline->GetOwnerIdentifier()));
            growbox->LogDataPoint(dp);
        } 
    }

    Event *Trigger::GetNextEventForSecondsInDay(long SecondsInDay)
    {

        for(std::vector<Event*>::iterator it = this->sequence->begin(); it != this->sequence->end(); ++it) {
            if ((*it)->GetSecondsInDay() >= SecondsInDay) {
                (*it)->SetRelativeSecondsToNext((*it)->GetSecondsInDay() - SecondsInDay);
                return (*it);
            }
        }
        this->sequence->at(0)->SetRelativeSecondsToNext(86400 - SecondsInDay);
        return this->sequence->at(0);
    }

    Event* Trigger::GetLastEventForSecondsInDay(long SecondsInDay)
    {
        for(std::vector<Event*>::reverse_iterator it = this->sequence->rbegin(); it != this->sequence->rend(); ++it) {
            if ((*it)->GetSecondsInDay() < SecondsInDay) {
                return (*it);
            }
        }
        return this->sequence->back();
    } 


    string Trigger::GetOwnerIdentifier()
    {
        return this->ownerIdentifier;
    }

    void Trigger::SetOwnerIdentifier(string ownerIdentifier)
    {
        this->ownerIdentifier = ownerIdentifier;

        for(std::vector<Event*>::iterator it = this->sequence->begin(); it != this->sequence->end(); ++it) {
            (*it)->SetTimelineIdentifier(ownerIdentifier);
        } 
    }


    void Trigger::Fire(Event *event, int currentTime)
    {
#ifdef __MACH__	    
	time_t maxValue = ULLONG_MAX;
#else 
	time_t maxValue = LONG_MAX;
#endif
        if (this->activeStartDate != maxValue) {

        }

        this->SetState(event->GetState());
       
                //we do not go through setter to avoid side effect 
        if(this->transient) 
        {
            this->state = Trigger::OFF;
        }
    }

    
    bool Trigger::PersistToStorage(sqlite3 *db)
    {

        sqlite3_stmt *res;
        bool result = false;

            //Check presence
        string sql = "SELECT * FROM triggers where uuid = @uuid;";
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
        if (rc != SQLITE_OK && rc!= SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
        
        int idx = sqlite3_bind_parameter_index(res, "@uuid");        
        sqlite3_bind_text(res, idx, this->GetIdentifier().c_str(), -1, SQLITE_TRANSIENT);
    
        int step = sqlite3_step(res);
        sqlite3_finalize(res);

        if (step == SQLITE_ROW) {
            
            sql = "UPDATE triggerss set (name, object_version, class_version) values (@name,  @object_version, @class_version);";
              cout <<  sqlite3_column_text(res, 0) << endl;
              cout << sqlite3_column_text(res, 1) << endl;
        } else {

            sql = "INSERT INTO triggers (uuid, name, transient, start_date, end_date, day_period, action_type, object_version, class_version) values (@uuid, @name, \
                        @transient, @start_date, @end_date, @day_period, @action_type, @object_version, @class_version);";
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
            if (rc != SQLITE_OK && rc != SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
           
            int idx = sqlite3_bind_parameter_index(res, "@uuid");        
            rc = sqlite3_bind_text(res, idx, this->GetIdentifier().c_str(), -1, SQLITE_TRANSIENT);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
           
            idx = sqlite3_bind_parameter_index(res, "@name");        
            rc =sqlite3_bind_text(res, idx, this->GetName().c_str(), -1, SQLITE_STATIC);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            idx = sqlite3_bind_parameter_index(res, "@transient");        
            rc = sqlite3_bind_int(res, idx, this->transient);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            idx = sqlite3_bind_parameter_index(res, "@start_date");        
            rc = sqlite3_bind_int(res, idx, this->activeStartDate);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            idx = sqlite3_bind_parameter_index(res, "@end_date");        
            rc = sqlite3_bind_int(res, idx, this->activeEndDate);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            idx = sqlite3_bind_parameter_index(res, "@day_period");        
            rc = sqlite3_bind_int(res, idx, this->dayPeriod);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            idx = sqlite3_bind_parameter_index(res, "@action_type");        
            rc =sqlite3_bind_text(res, idx, this->action->GetType().c_str(), -1, SQLITE_STATIC);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            idx = sqlite3_bind_parameter_index(res, "@class_version");        
            rc = sqlite3_bind_int(res, idx, this->GetClassVersion());
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            idx = sqlite3_bind_parameter_index(res, "@object_version");        
            rc = sqlite3_bind_int(res, idx, this->ObjectVersion);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            rc= sqlite3_step(res);
            if (rc != SQLITE_OK && rc != SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            sqlite3_finalize(res);

            int order=0;
            for(std::vector<Event*>::iterator it = this->sequence->begin(); it != this->sequence->end(); ++it) {

                sql = "INSERT INTO sequence_elements (tr_uuid, seq_order, duration, state) values (@tr_uuid, @seq_order, @duration, @state);";
                rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
                if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
         
                idx = sqlite3_bind_parameter_index(res, "@tr_uuid");        
                rc = sqlite3_bind_text(res, idx, this->GetIdentifier().c_str(), -1, SQLITE_TRANSIENT);
                if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
                idx = sqlite3_bind_parameter_index(res, "@seq_order");        
                rc = sqlite3_bind_int(res, idx, order);
                if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
                idx = sqlite3_bind_parameter_index(res, "@duration");        
                rc = sqlite3_bind_int(res, idx, (*it)->GetDuration());
                if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
                idx = sqlite3_bind_parameter_index(res, "@state");    
                int value = (*it)->GetState() == Base::ON ? 1 : 0;    
                rc = sqlite3_bind_int(res, idx, value);
                if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
                
                rc= sqlite3_step(res);
                if (rc != SQLITE_OK && rc != SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
                sqlite3_finalize(res);

                order++;
            }

            sql = "INSERT INTO timelines_triggers (tl_uuid, tr_uuid) values ( @tl_uuid, @tr_uuid);";
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
         
            idx = sqlite3_bind_parameter_index(res, "@tl_uuid");        
            rc = sqlite3_bind_text(res, idx, this->ownerIdentifier.c_str(), -1, SQLITE_STATIC);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}

            idx = sqlite3_bind_parameter_index(res, "@tr_uuid");        
            rc = sqlite3_bind_text(res, idx, this->GetIdentifier().c_str(), -1, SQLITE_TRANSIENT);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}

            rc= sqlite3_step(res);
            if (rc != SQLITE_OK && rc != SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            sqlite3_finalize(res);
        } 
        result = true;
        return result;

    }
}
