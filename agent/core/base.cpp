// The base class defines the default types

#include <string>
#include <iostream>
#include "base.h" 
#include <uuid/uuid.h>

using namespace std;


namespace twogreencows_core
{

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
            AllObjects.insert(this->identifier, this) 
            delete [] uuid;
        }

        void base::DumpObjects() 
        {
            unordered_map<string, base>::iterator it = AllObjects.begin();

        }

}
