// The base class defines the default types

#include <string>
#include <iostream>
#include <iomanip>
#include <uuid/uuid.h>

#include "Base.hpp" 

using namespace std;


namespace twogreencows_core
{

    unordered_map<string, Base*> *Base::AllObjects = new unordered_map<string, Base *>();   

    //constructor is not used really but we keep provision
    Base::Base()
    {
    }

    //Debug print member function utility
    string Base::GetIdentifier(bool shortPrint) const
    {
        if (shortPrint == false) {
            return identifier;
        } else {
            return identifier.substr(0,14)+"...";
        }
    }

    //To be called from children 
    void Base::SetUpIdentifier(std::string identifier)        
    {
        if (!identifier.empty()) {
            this->identifier = identifier;
        } else {
            uuid_t binuuid;
            char *uuid =  new char[37]; 

            uuid_generate_random(binuuid);
            uuid_unparse_lower(binuuid, uuid);
            this->identifier = this->GetClassPrefix()+"."+ string(uuid);
            delete [] uuid;
        }
        AllObjects->insert(make_pair(this->identifier, this)); 
    }


    //Static Debug Get all objects
    void Base::DumpObjects() 
    {
        unordered_map<string, Base*>::iterator it;
        for (it = Base::AllObjects->begin(); it != Base::AllObjects->end(); ++it) {
            cout << '[' << &(it->second) << "] " <<  it->first <<endl;
        }
    }

    //Static universal access to objects
    Base * Base::ObjectWithIdentifier(string identifier)
    {
        if (AllObjects->find(identifier) == AllObjects->end()) {
            return nullptr;
        }
        return Base::AllObjects->at(identifier);
    }

    void Base::DumpSequence(vector< pair<time_t, Base::State> > sequence, bool transient)
    {
        if (transient) {
            cout << "= Dump transient sequence: each ON is an event/each OFF is ignored" <<endl;
            
            long fullLength = 0;
            for(std::vector<pair<time_t, Base::State> >::iterator it = sequence.begin(); it != sequence.end(); ++it) {
                fullLength += (*it).first;
                long start_min_sec = fullLength % 3600;
                long start_s = start_min_sec % 60;
                long start_h = (fullLength - start_min_sec) / 3600;
                long start_m =  (start_min_sec - start_s) / 60;

                if ((*it).second == Base::ON) {
                    cout << "  -- ON at " << std::setfill('0') << std::setw(2) << start_h << ":" 
                       << std::setfill('0') << std::setw(2) << start_m << ":" 
                       << std::setfill('0') << std::setw(2) << start_s  << endl;
                } else {
                    cout << "  -- OFF ignored at " << start_h << "H:" << start_m << "m:" << start_s << "s" <<endl;
                }

            }
        } else {
            cout << "= Dump non-transient sequence, each ON is an state/each OFF also" << endl;
            long fullLength = 0;
            long lengthOFF = 0;
            long lengthON = 0; 
            for(std::vector<pair<time_t, Base::State> >::iterator it = sequence.begin(); it != sequence.end(); ++it) {
                long seqlength = (*it).first;
                long length_min_sec = seqlength % 3600;
                long start_min_sec = fullLength % 3600;
                long length_s = length_min_sec % 60;
                long start_s = start_min_sec % 60;
                long length_h = (seqlength - length_min_sec)/3600;
                long start_h = (fullLength - start_min_sec) / 3600;
                long length_m =  (length_min_sec - length_s)/ 60;
                long start_m =  (start_min_sec - start_s) / 60;

                fullLength += (*it).first;
                if ((*it).second == Base::ON) {
                    cout << "  -- ON  at " 
                         << std::setfill('0') << std::setw(2) << start_h <<  ":" 
                         << std::setfill('0') << std::setw(2) << start_m <<  ":" 
                         << std::setfill('0') << std::setw(2) << start_s  
                         << " for " 
                         << std::setfill('0') << std::setw(2) << length_h << ":"
                         << std::setfill('0') << std::setw(2) << length_m<< ":"  
                         << std::setfill('0') << std::setw(2) << length_s <<endl;
                    lengthON += (*it).first;
                } else {
                    cout << "  -- OFF at " 
                         << std::setfill('0') << std::setw(2) << start_h <<  ":" 
                         << std::setfill('0') << std::setw(2) << start_m <<  ":" 
                         << std::setfill('0') << std::setw(2) << start_s  
                         << " for " 
                         << std::setfill('0') << std::setw(2) << length_h << ":"
                         << std::setfill('0') << std::setw(2) <<  length_m<< ":"  
                         << std::setfill('0') << std::setw(2) << length_s <<endl;
                    lengthOFF += (*it).first;
                }

            }
            long on_min_sec = lengthON % 3600;
            long off_min_sec = lengthOFF % 3600;
            long on_s = on_min_sec % 60;
            long off_s = off_min_sec % 60;
            long on_h = (lengthON - on_min_sec)/3600;
            long off_h = (lengthOFF - off_min_sec) / 3600;
            long on_m =  (on_h - on_s)/ 60;
            long off_m =  (off_h - off_s) / 60;

            cout << "  == Total ON  is " 
                << std::setfill('0') << std::setw(2) << std::dec << on_h << ":"
                << std::setfill('0') << std::setw(2) << std::dec << on_m << ":"
                << std::setfill('0') << std::setw(2) << std::dec << on_s << endl;
            cout << "  == Total OFF is " 
                << std::setfill('0') << std::setw(2) << std::dec << off_h << ":"
                << std::setfill('0') << std::setw(2) << std::dec << off_m << ":"
                << std::setfill('0') << std::setw(2) << std::dec << off_s << endl;

            if (fullLength < 24 *3600) {                    
                cout << "  !! Sequence is invalid as it does not cover 24 hours";
            }
        }
    }

}



std::ostream &operator<<(std::ostream &os, twogreencows_core::Base const &b)
{
    return os << "[" << std::hex <<  &b << "] " << b.GetIdentifier();
}

