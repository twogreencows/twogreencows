#pragma once

#include "Base.hpp"

using namespace std;


namespace twogreencows_core
{
    class ActionRelay:public Action{
    
    public:
        
        ActionRelay();
        ~ActionRelay();
        unordered_map<DataPoint::DataPointKey, std::any> Execute(string triggerUUID);
        virtual string GetType() const;
    };
}
