#pragma once

#include "Action.hpp"

using namespace std;


namespace twogreencows_core
{
    class ActionTemperatureSensor:public Action{
    
    public:
        
        ActionTemperatureSensor();
        ~ActionTemperatureSensor();
        unordered_map<DataPoint::DataPointKey, std::any> Execute(string TriggerUUID);
        virtual string GetType() const;
    };
}
