//
//  Project: Twogreencows Agent
//  File: GrowboxManager.cpp
//
//  Description: Implementation file for GrowboxManager. This component is responsible
//  for the management of growboxes. It also handles communication with persistent
//  storage.In version 1 only one growbox can beb active at a time.
//
//  Copyright TwoGreenCows 2020
//


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include "GrowboxManager.hpp" 
#include "Growbox.hpp" 
#include "Timeline.hpp" 
#include "Trigger.hpp" 

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>

namespace twogreencows_core
{

    GrowboxManager *GrowboxManager::SharedManager=nullptr;

        //Creator: ensure all folder hierarchy is here
    GrowboxManager::GrowboxManager()
    {
        //Ensure top level folder is here
        string homedir = getenv("HOME");
        if (homedir.empty() ) {
            struct passwd *pw = getpwuid(getuid());
            homedir = pw->pw_dir;
        }

        string twogreencowsMainFolderPath = homedir + "/.twogreencows/";

        struct stat st = {0};
        if (stat(twogreencowsMainFolderPath.c_str(), &st) == -1) {
            mkdir(twogreencowsMainFolderPath.c_str(), 0755);
        }
        this->TwoGreenCowsMainFolderPath = twogreencowsMainFolderPath;

        this->OpenedDB = NULL;
        this->OpenedIdentifier = "";
    }

        //Singleton access
    GrowboxManager* GrowboxManager::GetSharedManager()
    {
        if (nullptr == SharedManager) {
            SharedManager = new GrowboxManager();
        }
        return SharedManager;
    }

        //Get storage path for a growboz designed by its identifier
    std::string GrowboxManager::GetTopFolderPathForGrowboxIdentifier(std::string GrowboxIdentifier)
    {      
        string prefix = "growbox";
        bool hasProperPrefix = std::mismatch(prefix.begin(), prefix.end(), GrowboxIdentifier.begin(), GrowboxIdentifier.end()).first == prefix.end();
        if (!hasProperPrefix) {
            return "";
        }
        return this->TwoGreenCowsMainFolderPath +GrowboxIdentifier + "/";
    }

        //If a Growbox has been run already it should have an identifier
    std::string GrowboxManager::GetLastActiveGrowboxIdentifier()
    {        
        std::string result= "";
        std::string fullfilepath = this->TwoGreenCowsMainFolderPath + "tgc_runtime.json";

        ifstream ifs(fullfilepath.c_str());
        if ( !ifs.is_open() )
        {
            cerr << "Could not open file for reading!\n";
            return result;
        }

        IStreamWrapper isw(ifs);
        Document doc;
        doc.ParseStream(isw);

        if (!doc.IsObject()) {
            cerr << "Invalid File Format" << endl;
            return result;
        }

        if (!doc.HasMember("last_growbox_identifier")) {
            cerr << "Missing last_growbox_identifier key" << endl;
            return result;
        }
       
        const Value& a = doc["last_growbox_identifier"];
        if (!a.IsString()) {
            cerr << "Value for last_growbox_identifier has wrong type" << endl;
            return result;
        }

        string prefix = "growbox";
        string identifier = a.GetString();
        bool hasProperPrefix = std::mismatch(prefix.begin(), prefix.end(), identifier.begin(), identifier.end()).first == prefix.end();
        if (!hasProperPrefix) {
            cerr << "Improper syntax for last_growbox_identifier value" << endl;
            return result;
        }
        result = identifier;

        return result;
    }

        //Save the identifier of the last run growbox
    void GrowboxManager::SetLastActiveGrowboxIdentifier(std::string GrowboxIdentifier)
    {
        std::string fullfilepath = this->TwoGreenCowsMainFolderPath + "tgc_runtime.json";


        StringBuffer s;
        PrettyWriter<StringBuffer> writer(s);
    
        writer.StartObject();               // Between StartObject()/EndObject(), 
        writer.Key("last_growbox_identifier");                // output a key,
        writer.String(GrowboxIdentifier);             // follow by a value.
        writer.EndObject();
        
        std::ofstream of (fullfilepath);
        
        of << s.GetString();
        if (!of.good()) {
            std::cerr << "Error writing to file";   
        }
    }

        
    void GrowboxManager::BuildDataBaseForIdentifier(string GrowboxIdentifier)
    {
        sqlite3 *db;

        std::string GrowboxFolderPath = this->TwoGreenCowsMainFolderPath + GrowboxIdentifier +"/";     
        
        //ensure folder is here
        struct stat st = {0};
        if (stat(GrowboxFolderPath.c_str(), &st) == -1) {
            mkdir(GrowboxFolderPath.c_str(), 0755);
        }

        //Open DB - will create it if not present
        std::string DBPath = GrowboxFolderPath +  "storage.sqlite";
        int rc = sqlite3_open(DBPath.c_str(), &db);
        if (rc != SQLITE_OK) {
            cerr <<  "Cannot open database: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
        }
        
        //fill with statement
        std::vector<std::string> SQLStatements;
        SQLStatements.push_back("PRAGMA foreign_keys = ON;");
        SQLStatements.push_back("PRAGMA user_version = 1;");
        SQLStatements.push_back("CREATE TABLE IF NOT EXISTS growboxes(uuid TEXT NOT NULL PRIMARY KEY, name TEXT NOT NULL, server_ipv4 TEXT NOT NULL,\
                    object_version INTEGER NOT NULL, class_version INTEGER NOT NULL);");
        SQLStatements.push_back("CREATE TABLE IF NOT EXISTS timelines(uuid TEXT NOT NULL PRIMARY KEY, name TEXT NOT NULL,\
                    object_version INTEGER NOT NULL, class_version INTEGER NOT NULL);");
        SQLStatements.push_back("CREATE TABLE IF NOT EXISTS growboxes_timelines(gb_uuid TEXT REFERENCES growboxes, tl_uuid TEXT REFERENCES timelines);");
        SQLStatements.push_back("CREATE TABLE IF NOT EXISTS triggers(uuid TEXT NOT NULL PRIMARY KEY, name TEXT NOT NULL, \
            transient INTEGER NOT NULL,start_date INTEGER NOT NULL, end_date INTEGER MOT NULL, day_period INTEGER NOT NULL, action_type TEXT NOT NULL,\
                 object_version INTEGER NOT NULL, class_version INTEGER NOT NULL);");
        SQLStatements.push_back("CREATE TABLE IF NOT EXISTS sequence_elements(id INTEGER PRIMARY KEY AUTOINCREMENT, tr_uuid TEXT REFERENCES triggers, \
                seq_order INTEGER, duration INTEGER NOT NULL, state INTEGER NOT NULL);");
        SQLStatements.push_back("CREATE TABLE IF NOT EXISTS timelines_triggers(tl_uuid TEXT REFERENCES timelines, tr_uuid TEXT REFERENCES triggers);");

        //Do creation
        for(std::vector<std::string>::iterator it = SQLStatements.begin(); it != SQLStatements.end(); ++it) {
            char *err_msg = NULL;
            rc = sqlite3_exec(db, (*it).c_str(), 0, 0, &err_msg); 
            //cout << (*it) << " " << rc << endl;
            if (rc != SQLITE_OK ) {
                cerr <<  "SQL error: " << (*it) << " " << err_msg <<endl;
                sqlite3_free(err_msg);        
                sqlite3_close(db);
            }
        }
        
        sqlite3_close(db);
    }

    void GrowboxManager::OpenDataBaseForIdentifier(std::string GrowboxIdentifier)
    {
       bool proceed = false;
       if (0 == this->OpenedIdentifier.compare("")) {
           proceed =true;
        } else if (0 != this->OpenedIdentifier.compare(GrowboxIdentifier)){
                sqlite3_close(this->OpenedDB);
                this->OpenedDB = nullptr;
                this->OpenedIdentifier = "";
                proceed = true;
        }
        if(proceed) {
            this->OpenedIdentifier = GrowboxIdentifier;
            std::string DBPath = this->TwoGreenCowsMainFolderPath + GrowboxIdentifier +"/storage.sqlite";     
            int rc = sqlite3_open(DBPath.c_str(), &(this->OpenedDB));
            if (rc != SQLITE_OK) {
                cerr <<  "Cannot open database: " << sqlite3_errmsg(this->OpenedDB) << endl;
                this->OpenedDB = nullptr;
                this->OpenedIdentifier = "";
            }
        }
    }

    void GrowboxManager::CloseDataBaseForIdentifier(std::string GrowboxIdentifier)
    {
       if (0 == this->OpenedIdentifier.compare(GrowboxIdentifier)) {
            sqlite3_close(this->OpenedDB);
            this->OpenedDB = nullptr;
            this->OpenedIdentifier = ""; 
       }
    }

    sqlite3 *GrowboxManager::GetOpenedDataBase()
    {
        return this->OpenedDB;
    }       

    Growbox *GrowboxManager::CreateGrowboxFromDescriptorFile(string PathToDescriptorFile, bool PersistToStorage)
    {    
        cerr << "== Open from file " + PathToDescriptorFile << endl;
        Growbox *result = nullptr;
        unordered_map<string,string > dependancies;


        ifstream ifs(PathToDescriptorFile.c_str());
        if ( !ifs.is_open() ){ cerr << "Could not open file for reading!\n";return result;}

        IStreamWrapper isw(ifs);
        Document doc;
        doc.ParseStream(isw);
        
        if (!doc.IsObject()) {cerr << "Invalid File Format" << endl; return result;}
        if (!doc.HasMember("objects")) { cerr << "Missing objects key" << endl; return result;}
       
        const Value& objects = doc["objects"];
        if (!objects.IsArray()) { cerr << "Invalid objects data" <<endl; return result;}
    
            //We will round 3 times over objects to pick them in proper order
        for (SizeType i = 0; i < objects.Size(); i++) {
            if (!objects[i].IsObject()) { cerr << "Invalid data for object at index " << i <<endl; continue; }
            if (!objects[i].HasMember("type")) { cerr << "No type for object at index" << i << endl; continue;}
            if (!objects[i]["type"].IsString()) { cerr << "Wrong type kinf for object at index" << i << endl; continue;}

            string type = objects[i].GetObject()["type"].GetString();
            
            if (0 == type.compare("growbox")) {
                if (nullptr != result) {
                    cerr << "WARNING only one growbox is allowed" << endl;
                    continue;
                }

                unordered_map<std::string, std::string> parameters = unordered_map<std::string, std::string>();
                if (objects[i].HasMember("name") && objects[i]["name"].IsString()) {
                    parameters.insert(make_pair("name",  objects[i]["name"].GetString()));
                } else {
                    cerr << "WARNING: no name in growbox object" << endl;
                }
                if (objects[i].HasMember("server_ipv4") && objects[i]["server_ipv4"].IsString()) {
                    parameters.insert(make_pair("server_ipv4",  objects[i]["server_ipv4"].GetString()));
                } else {
                    cerr << "WARNING: no server_ipv4 in growbox object" << endl;
                }

                result  = new Growbox(parameters);
                if (objects[i].HasMember("timelines") && objects[i]["timelines"].IsArray()) {
                    const Value& timelines =  objects[i]["timelines"];
                    for (SizeType j = 0; j < timelines.Size(); j++) {
                        if (timelines[j].IsString()) {
                            dependancies.insert(make_pair(timelines[j].GetString(), result->GetIdentifier())); 
                        } else {
                            cerr << "WARNING: improper reference type in file" << endl;
                        }
                    } 
                }
            }
        }

        if (nullptr == result) {
            return result;
        }
        if (PersistToStorage) {
            this->BuildDataBaseForIdentifier(result->GetIdentifier());
            this->OpenDataBaseForIdentifier(result->GetIdentifier());
            result->PersistToStorage(this->GetOpenedDataBase());
        }

        for (SizeType i = 0; i < objects.Size(); i++) {
            if (!objects[i].IsObject()) { cerr << "Invalid data for object at index " << i <<endl; continue; }
            if (!objects[i].HasMember("type")) { cerr << "No type for object at index" << i << endl; continue;}
            if (!objects[i]["type"].IsString()) { cerr << "Wrong type kinf for object at index" << i << endl; continue;}

            string type = objects[i]["type"].GetString();
 
            if (0 == type.compare("timeline")) {

                if (objects[i].HasMember("tgc_fileref") && objects[i]["tgc_fileref"].IsString()) {
                    string tgcfileref = objects[i]["tgc_fileref"].GetString();
                    string OwningGrowboxIdentifier = dependancies.at(tgcfileref);
                    
                    string name = tgcfileref; //Default is tgcfileref

                    if (objects[i].HasMember("name") && objects[i]["name"].IsString()) {
                        name = objects[i]["name"].GetString();
                    } else {
                        cerr << "WARNING: no name precised on timeline " << name <<endl;
                    }

                    if (0 == OwningGrowboxIdentifier.compare(result->GetIdentifier())) {
                        Timeline *tl= new Timeline(name);
                        result->AddTimeline(tl);

                        if (PersistToStorage) {
                            tl->PersistToStorage(this->GetOpenedDataBase());     
                        }
                        if (objects[i].HasMember("triggers") && objects[i]["triggers"].IsArray()) {
                               const Value& triggers =  objects[i].GetObject()["triggers"];
                                for (SizeType j = 0; j < triggers.Size(); j++) 
                                {
                                    dependancies.insert(make_pair(triggers[j].GetString(), tl->GetIdentifier())); 
                                } 
                        }
                    }   
                } 
            }
        }

        for (SizeType i = 0; i < objects.Size(); i++) {// Uses SizeType instead of size_t
            if (!objects[i].IsObject()) { cerr << "Invalid data for object at index " << i <<endl; continue; }
            if (!objects[i].HasMember("type")) { cerr << "No type for object at index" << i << endl; continue;}
            if (!objects[i]["type"].IsString()) { cerr << "Wrong type kinf for object at index" << i << endl; continue;}
        
            
            string type = objects[i]["type"].GetString();
            
            if (0 == type.compare("trigger")) {
                
                if (objects[i].HasMember("tgc_fileref") && objects[i]["tgc_fileref"].IsString() 
                        && objects[i].HasMember("action_type") && objects[i]["action_type"].IsString()) {
                    string tgcfileref = objects[i]["tgc_fileref"].GetString();
                    string action_type = objects[i]["action_type"].GetString();
                    string name = tgcfileref; //Default is tgcfileref
                    
                    Timeline*tl = static_cast<Timeline *>(Base::ObjectWithIdentifier(dependancies.at(tgcfileref)));
                    if (nullptr != tl) {
    
                        if (objects[i].HasMember("name") && objects[i]["name"].IsString()) {
                            name = objects[i]["name"].GetString();
                        } else {
                            cerr << "WARNING: no name precised on timeline " << name <<endl;
                        }

    
                        bool transient = false;
                        if (objects[i].HasMember("transient") && objects[i]["transient"].IsBool()) {
                            transient = objects[i]["transient"].GetBool();
                        }
                        Action *action = Action::CreateActionForType(action_type, result->GetIdentifier());

                        vector< pair<time_t, Base::State> > sequence;
                        if (objects[i].HasMember("sequence") && objects[i]["sequence"].IsArray()) {
                            const Value &fileSequence = objects[i]["sequence"];

                            for (SizeType j = 0; j < fileSequence.Size(); j++) {
                                float aDuration = fileSequence[j]["duration"].GetFloat();
                                string aValue = fileSequence[j]["value"].GetString();
                                if (0 == aValue.compare("ON")) {
                                    sequence.push_back(make_pair(aDuration, Base::ON));
                                } else if (0 == aValue.compare("OFF")) {
                                    sequence.push_back(make_pair(aDuration, Base::OFF));
                                }
                            }
                        }

                        Trigger *tr = new Trigger(name, action, sequence, transient);
                        tl->ScheduleTrigger(tr);
                        if (PersistToStorage) {
                            tr->PersistToStorage(this->GetOpenedDataBase());
                        }
                    }
                }  
            }
        }
        
        this->CloseDataBaseForIdentifier(result->GetIdentifier());
        return result;
    }


    Growbox *GrowboxManager::CreateGrowboxFromPersistentStorage(std::string GrowboxIdentifier) 
    {
        Growbox *result = nullptr;
        Growbox *box = nullptr;
        std::vector<std::string> TimelinesUUID;

        //cout << "reading from DB " <<endl;
        this->OpenDataBaseForIdentifier(GrowboxIdentifier);
        sqlite3 *db = this->GetOpenedDataBase();    
        if (nullptr == db) {
            cerr << "ERROR invalid identifier, no database found" << endl;
            return nullptr;
        }
        
        sqlite3_stmt *res;

            //Check presence of growbox
        string sql = "SELECT uuid, name,server_ipv4 FROM growboxes where uuid = @uuid; ";
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
        if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
        int idx = sqlite3_bind_parameter_index(res, "@uuid");        
        rc = sqlite3_bind_text(res, idx, GrowboxIdentifier.c_str(), -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; return result;}
        int step = sqlite3_step(res);
        if (step != SQLITE_ROW) { this->CloseDataBaseForIdentifier(GrowboxIdentifier); return result; }


        string GBUUID =  string(reinterpret_cast<const char*>(sqlite3_column_text(res, 0)));
        //cout << GBUUID << " read" <<endl;
        string GBName =  string(reinterpret_cast<const char*>(sqlite3_column_text(res, 1)));
        string GBServerIPV4 =  string(reinterpret_cast<const char*>(sqlite3_column_text(res, 2)));
        
        unordered_map<std::string, std::string> parameters = unordered_map<std::string, std::string>();
        parameters.insert(make_pair("server_ipv4", GBServerIPV4));
        parameters.insert(make_pair("identifier", GBUUID));
        parameters.insert(make_pair("name", GBName));
        box = new Growbox(parameters);
        sqlite3_finalize(res); 

        //cout << "done " <<endl;
        sql = "SELECT tl_uuid from growboxes_timelines WHERE gb_uuid = @gb_uuid";
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
        if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
            this->CloseDataBaseForIdentifier(GrowboxIdentifier);return result;}
        idx = sqlite3_bind_parameter_index(res, "@gb_uuid");        
        rc = sqlite3_bind_text(res, idx, GrowboxIdentifier.c_str(), -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
            this->CloseDataBaseForIdentifier(GrowboxIdentifier);return result;}

        for (;;) {
            step = sqlite3_step(res);         
            if (step == SQLITE_DONE) { break;}
            if (step != SQLITE_ROW) { cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
               this->CloseDataBaseForIdentifier(GrowboxIdentifier); return result;}
            //cout << "one timeline" << endl;
            string TLUUID =  string(reinterpret_cast<const char*>(sqlite3_column_text(res, 0)));
            //cout << TLUUID <<endl;
            TimelinesUUID.push_back(TLUUID);
        }
        sqlite3_finalize(res);


        for(std::vector<std::string>::iterator it = TimelinesUUID.begin(); it != TimelinesUUID.end(); ++it) {
            Timeline *tl;

            sql = "SELECT name from timelines WHERE uuid = @uuid";
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
            if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
                    this->CloseDataBaseForIdentifier(GrowboxIdentifier);return result;}
            idx = sqlite3_bind_parameter_index(res, "@uuid");        
            rc = sqlite3_bind_text(res, idx, (*it).c_str() , -1, SQLITE_TRANSIENT);
            if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
                    this->CloseDataBaseForIdentifier(GrowboxIdentifier);return result;}
            step = sqlite3_step(res)                        ;
            if (step == SQLITE_ROW) {
                string TLName =  string(reinterpret_cast<const char*>(sqlite3_column_text(res, 0)));
                tl = new Timeline(TLName, *it);
                box->AddTimeline(tl);
            }
            sqlite3_finalize(res);

            std::vector<std::string> TriggersUUID;
            sql = "SELECT tr_uuid from timelines_triggers WHERE tl_uuid = @tl_uuid";
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
            if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
                    this->CloseDataBaseForIdentifier(GrowboxIdentifier);return result;}
            idx = sqlite3_bind_parameter_index(res, "@tl_uuid");        
            rc = sqlite3_bind_text(res, idx, (*it).c_str() , -1, SQLITE_TRANSIENT);
            if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
                    this->CloseDataBaseForIdentifier(GrowboxIdentifier);return result;}
            for (;;) {
                step = sqlite3_step(res);         
                if (step == SQLITE_DONE) { break;}
                if (step != SQLITE_ROW) { cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
                    this->CloseDataBaseForIdentifier(GrowboxIdentifier); return result;}

                string TRUUID =  string(reinterpret_cast<const char*>(sqlite3_column_text(res, 0)));
                //cout <<TRUUID << endl;
                TriggersUUID.push_back(TRUUID);
            }
            sqlite3_finalize(res);


            for(std::vector<std::string>::iterator itt = TriggersUUID.begin(); itt != TriggersUUID.end(); ++itt) {

                vector< pair<time_t, Base::State> > sequence;
                sql = "SELECT seq_order, duration, state  from sequence_elements  WHERE tr_uuid = @tr_uuid order by seq_order";
                rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
                if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
                    this->CloseDataBaseForIdentifier(GrowboxIdentifier);return result;}
                idx = sqlite3_bind_parameter_index(res, "@tr_uuid");        
                rc = sqlite3_bind_text(res, idx, (*itt).c_str() , -1, SQLITE_TRANSIENT);
                if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
                    this->CloseDataBaseForIdentifier(GrowboxIdentifier);return result;}
                for (;;) {
                    step = sqlite3_step(res);         
                    if (step == SQLITE_DONE) { break;}
                    if (step != SQLITE_ROW) { cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
                        this->CloseDataBaseForIdentifier(GrowboxIdentifier); return result;}

                    int duration =  sqlite3_column_int(res, 1);
                    Base::State state = (sqlite3_column_int(res, 2) == 1) ? Base::ON : Base::OFF;
                    sequence.push_back(make_pair(duration, state));
                }
                sqlite3_finalize(res);
                
                sql = "SELECT name, transient, start_date, end_date, day_period, action_type  from triggers  WHERE uuid = @uuid";
                rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
                if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
                    this->CloseDataBaseForIdentifier(GrowboxIdentifier);return result;}
                idx = sqlite3_bind_parameter_index(res, "@uuid");        
                rc = sqlite3_bind_text(res, idx, (*itt).c_str() , -1, SQLITE_TRANSIENT);
                if (rc != SQLITE_OK && rc!=SQLITE_DONE) {  cerr <<  "Failed to execute statement (" << sql << ")" <<  sqlite3_errmsg(db) << endl; 
                    this->CloseDataBaseForIdentifier(GrowboxIdentifier);return result;}
                
                step = sqlite3_step(res)                        ;
                if (step == SQLITE_ROW) {
                    string TRName =  string(reinterpret_cast<const char*>(sqlite3_column_text(res, 0)));
                    bool transient = (sqlite3_column_int(res, 1) == 1) ? true : false;
    
                    string TRActionType =  string(reinterpret_cast<const char*>(sqlite3_column_text(res, 5)));
                    Action *action = Action::CreateActionForType(TRActionType, box->GetIdentifier());
                    Trigger *tr = new Trigger(TRName, action, sequence, transient, (*itt));
                    tl->ScheduleTrigger(tr);
                }
                sqlite3_finalize(res);
          
            }
        }
        result = box;
        this->CloseDataBaseForIdentifier(GrowboxIdentifier);
        return result;
    }
}
