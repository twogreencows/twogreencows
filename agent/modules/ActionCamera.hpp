#pragma once

#include "Action.hpp"

using namespace std;


namespace twogreencows_core
{
    class ActionCamera:public Action{
            string identifier;
    
    public:
        
        ActionCamera();
        ~ActionCamera();
        virtual string GetType() const;
        unordered_map<DataPoint::DataPointKey, std::any> Execute(string TriggerUUID);
    };
}
