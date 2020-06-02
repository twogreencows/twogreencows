//
//  Project: Twogreencows Agent
//  File: Base.hpp
//
//  Description: Interface file for Base. This class is the Base class: it is responsible
//  for the management of object identification, versioning and general access. 
//  Some non persistent objects like DataPoint may not derive from the base class
//
//  Copyright TwoGreenCows 2020
//


#pragma once

#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

namespace twogreencows_core
{
    class Base{
            string identifier;
            static unordered_map<string, Base*> *AllObjects;     
        protected:
            int ObjectVersion;
        public:
        enum State{
            OFF,
            ON
        };

        virtual string  GetClassPrefix() const = 0; 
        virtual int GetClassVersion() const = 0; 
        
        string GetIdentifier(bool shortPrint=false) const;
        void SetUpIdentifier(std::string idenfitier="");
        
        static void DumpObjects();
        static Base *ObjectWithIdentifier(string identifier);
        static void DumpSequence(vector<pair<time_t, Base::State>> sequence, bool transient = false);
        Base();
    };
}


std::ostream &operator<<(std::ostream &os, twogreencows_core::Base const &b); 
