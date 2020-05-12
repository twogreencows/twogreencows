// The base class defines the default types

#include <string>
#include <iostream>
#include <uuid/uuid.h>

#include "Base.hpp" 

using namespace std;


namespace twogreencows_core
{
        unordered_map<string, Base*> *(Base::AllObjects) = new unordered_map<string, Base *>();   

            //constructor is not used really but we keep provision
        Base::Base()
        {
        }

            //member
        string Base::GetIdentifier() const
        {
            return identifier;
        }


        void Base::SetUpIdentifier()        
        {
            uuid_t binuuid;
            char *uuid =  new char[37]; 

            uuid_generate_random(binuuid);
            uuid_unparse_lower(binuuid, uuid);
            this->identifier = this->GetPrefix()+"."+ uuid;
            AllObjects->insert(make_pair(this->identifier, this)); 
            delete [] uuid;
        }

            //Stream operatpr overriding
        std::ostream &operator<<(std::ostream &os, Base const &m) 
        {
            return os << "[" << (void*) &m <<"] " << m.GetIdentifier();
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
            return Base::AllObjects->at(identifier);
        }
}
