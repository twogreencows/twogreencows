#pragma once


using namespace std;

#include <unordered_map>
#include <vector>

namespace twogreencows_core
{
    class Base{
            string identifier;
            static unordered_map<string, Base*> *AllObjects;     
        public:

        enum State{
            OFF,
            ON
        };

        virtual string  GetPrefix() const = 0 ; 
        string GetIdentifier(bool shortPrint=false) const;
        void SetUpIdentifier();
        static void DumpObjects();
        static Base *ObjectWithIdentifier(string identifier);
        static void DumpSequence(vector<pair<time_t, Base::State>> sequence, bool transient = false);
        Base();
    };
}


std::ostream &operator<<(std::ostream &os, twogreencows_core::Base const &b); 
