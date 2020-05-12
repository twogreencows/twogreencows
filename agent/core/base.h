#pragma once


using namespace std;

#include <unordered_map>

namespace twogreencows_core
{
    class base{
            string identifier;
            static unordered_map<string, base> AllObjects;        
        public:

        virtual string  GetPrefix() const = 0 ; 
        string GetIdentifier() const;
        void SetUpIdentifier();
        static void DumpObjects();
        base();
    };
}
