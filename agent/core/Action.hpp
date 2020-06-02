#pragma once

#include "Base.hpp"
#include "DataPoint.hpp"

using namespace std;


namespace twogreencows_core
{
    class Action:public Base{
            string identifier;
            string OwnerIdentifier="";    
    public:
        
        Action(string growboxIdentifier);
        ~Action();
        string GetOwnerIdentifier();
        virtual unordered_map<DataPoint::DataPointKey, std::any> Execute(string TriggerUUID);
        virtual string GetClassPrefix() const;
        virtual int GetClassVersion() const;
        virtual string GetType() const = 0;

        static Action *CreateActionForType(string actionType, string growboxIdentifier);
    };
}
