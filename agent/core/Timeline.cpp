
#include <string>
#include <iostream>
#include "Base.hpp"
#include "Timeline.hpp" 
#include "Trigger.hpp"

using namespace std;


namespace twogreencows_core
{

    string Timeline::GetClassPrefix() const
    {
        return "timeline";
    }

    int Timeline::GetClassVersion() const
    {
        return 1;
    }


    Timeline::Timeline(string name, std::string identifier)
    {
        this->SetUpIdentifier(identifier); 
        this->ObjectVersion = 1; 
        this->name = name;
        this->AllTriggers = new vector<Trigger*>();
        cerr << " = Create Timeline" + this->GetIdentifier()+ "("+this->GetName()+")" << endl;
    }

    Timeline::~Timeline()
    {
        cout << "timeline destructor";
    }

    string Timeline::GetName()
    {
        return this->name;
    }

    string Timeline::GetOwnerIdentifier()
    {
        return this->ownerIdentifier;
    }

    void Timeline::SetOwnerIdentifier(string ownerIdentifier)
    {
        this->ownerIdentifier = ownerIdentifier;
    }

    void Timeline::ScheduleTrigger(Trigger *trigger)
    {
        trigger->SetOwnerIdentifier(this->GetIdentifier());

        AllTriggers->push_back(trigger);

        //cout <<"   -->  Now we have " << AllTriggers->size() << endl;
    }

    std::vector<Event*> Timeline::StartTriggers(int kq, long timeInSeconds)
    {
        //Build Events for day
        cout << "  + Starting timeline " << this->GetName() << "(" << this->GetIdentifier(true) << ")" << endl;
        std::vector<Event *> result ; 
        //set trigger to the state they should havee if not transient
        for(std::vector<Trigger *>::iterator it = this->AllTriggers->begin(); it != this->AllTriggers->end(); ++it) {
            Event *triggerNextEvent = (*it)->GetNextEventForSecondsInDay(timeInSeconds);
            Event *triggerLastEvent = (*it)->GetLastEventForSecondsInDay(timeInSeconds);
            if ( (triggerLastEvent != NULL) && ((*it)->GetTransient() == false) ) {
                (*it)->Fire(triggerLastEvent, timeInSeconds);
            }
            result.push_back(triggerNextEvent);
        }
        return result;
    }

    std::vector<Event *> Timeline::GetNextEventForTriggers(int kq, long timeInSeconds)
    {
        std::vector<Event *> result; 
        for(std::vector<Trigger *>::iterator it = this->AllTriggers->begin(); it != this->AllTriggers->end(); ++it) {
            Event *triggerNextEvent = (*it)->GetNextEventForSecondsInDay(timeInSeconds);
            result.push_back(triggerNextEvent);
        }
        return result;
    }

    bool Timeline::PersistToStorage(sqlite3 *db)
    {

        sqlite3_stmt *res;
        bool result = false;

        //Check presence
        string sql = "SELECT * FROM timelines where uuid = @uuid;";
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
        if (rc != SQLITE_OK && rc!= SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
        
        int idx = sqlite3_bind_parameter_index(res, "@uuid");        
        sqlite3_bind_text(res, idx, this->GetIdentifier().c_str(), -1, SQLITE_TRANSIENT);

        int step = sqlite3_step(res);
        sqlite3_finalize(res);

        if (step == SQLITE_ROW) {
            sql = "UPDATE timelines set (name, object_version, class_version) values (@name,  @object_version, @class_version);";
            cout <<  sqlite3_column_text(res, 0) << endl;
            cout << sqlite3_column_text(res, 1) << endl;
        } else {
            sql = "INSERT INTO timelines (uuid, name, object_version, class_version) values ( @uuid, @name, @object_version, @class_version);";
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}

            int idx = sqlite3_bind_parameter_index(res, "@uuid");        
            rc = sqlite3_bind_text(res, idx, this->GetIdentifier().c_str(), -1, SQLITE_TRANSIENT);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}

            idx = sqlite3_bind_parameter_index(res, "@name");        
            rc =sqlite3_bind_text(res, idx, this->GetName().c_str(), -1, SQLITE_STATIC);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}

            idx = sqlite3_bind_parameter_index(res, "@object_version");        
            rc = sqlite3_bind_int(res, idx, this->ObjectVersion);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}

            idx = sqlite3_bind_parameter_index(res, "@class_version");        
            rc = sqlite3_bind_int(res, idx, this->GetClassVersion());
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}

            rc= sqlite3_step(res);
            if (rc != SQLITE_OK && rc != SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}

            sqlite3_finalize(res);

            sql = "INSERT INTO growboxes_timelines (gb_uuid, tl_uuid) values (@gb_uuid, @tl_uuid);";
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
            
            idx = sqlite3_bind_parameter_index(res, "@gb_uuid");      
            rc = sqlite3_bind_text(res, idx, this->ownerIdentifier.c_str(), -1, SQLITE_TRANSIENT);
            if (rc != SQLITE_OK) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}

            idx = sqlite3_bind_parameter_index(res, "@tl_uuid");        
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

