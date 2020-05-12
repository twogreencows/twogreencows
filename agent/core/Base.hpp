#pragma once


using namespace std;

#include <unordered_map>

namespace twogreencows_core
{
    class Base{
            string identifier;
            static unordered_map<string, Base*> *AllObjects;     
        public:

        virtual string  GetPrefix() const = 0 ; 
        string GetIdentifier() const;
        void SetUpIdentifier();
        static void DumpObjects();
        Base *ObjectWithIdentifier(string identifier);
        Base();
    };
}
