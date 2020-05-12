// The base class defines the default types

#include <string>
#include <iostream>
#include <uuid/uuid.h>

#include "base.h" 

using namespace std;


namespace twogreencows_core
{
        unordered_map<string, base*> *(base::AllObjects) = new unordered_map<string, base *>();   
        string base::GetIdentifier() const
        {
            return identifier;
        }

        base::base()
        {
        }

        void base::SetUpIdentifier()        
        {
            uuid_t binuuid;
            char *uuid =  new char[37]; 

            uuid_generate_random(binuuid);
            uuid_unparse_lower(binuuid, uuid);
            this->identifier = this->GetPrefix()+"."+ uuid;
            AllObjects->insert(make_pair(this->identifier, this)); 
            delete [] uuid;
        }

        void base::DumpObjects() 
        {
            unordered_map<string, base*>::iterator it;
            for (it = base::AllObjects->begin(); it != base::AllObjects->end(); ++it) {
                cout << '[' << &(it->second) << "] " <<  it->first <<endl;
            }
        }

        std::ostream &operator<<(std::ostream &os, base const &m) {
            return os << "[" << (void*) &m <<"] " << m.GetIdentifier();
        }

}
