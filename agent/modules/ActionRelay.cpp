#include "Base.hpp"
#include "Growbox.hpp"
#include "Action.hpp"
#include "ActionRelay.hpp"
#include "DataPoint.hpp"
#include "Trigger.hpp"
#include "Timeline.hpp"

#include <iostream>

namespace twogreencows_core
{
    ActionRelay::ActionRelay(string growboxIdentifier) : Action(growboxIdentifier)
    {
    }
    
        
    ActionRelay::~ActionRelay()
    {
    }

    string ActionRelay::GetType() const
    {
        return "relay";
    }

    unordered_map<DataPoint::DataPointKey, std::any> ActionRelay::Execute(string TriggerUUID)
    {
        cerr << "From relay senspr" << endl;
        Trigger *tmpTrigger = static_cast<Trigger *> (Base::ObjectWithIdentifier(TriggerUUID));
        int value = 0.0;
        if (tmpTrigger->GetState() == Base::ON) {
            value = 1.0;
        }

        unordered_map<DataPoint::DataPointKey, std::any> result =  unordered_map<DataPoint::DataPointKey, std::any>(
                 {{DataPoint::SUBJECT_NAME, tmpTrigger->GetName()},
                 {DataPoint::SENSOR_KIND, string("relay")},
                 {DataPoint::SENSOR_BRAND, string("none")},
                 {DataPoint::SENSOR_MODEL, string("generic")},
                 {DataPoint::SENSOR_VALUE, value},
                 {DataPoint::SENSOR_UNIT, string("bit")}}); 
        return result;
    }
}    

