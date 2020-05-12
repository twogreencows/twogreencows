
#include "Base.hpp"
#include "Action.hpp"
#include "ActionTemperatureSensor.hpp"
#include "DataPoint.hpp"
#include "Trigger.hpp"
#include "Timeline.hpp"

namespace twogreencows_core
{
    ActionTemperatureSensor::ActionTemperatureSensor()
    {
    }
    
        
    ActionTemperatureSensor::~ActionTemperatureSensor()
    {
    }

    string ActionTemperatureSensor::GetType() const
    {
        return "temperature";
    }

    unordered_map<DataPoint::DataPointKey, std::any> ActionTemperatureSensor::Execute(string TriggerUUID)
    {            
        Trigger *tmpTrigger = static_cast<Trigger *> (Base::ObjectWithIdentifier(TriggerUUID));
        unordered_map<DataPoint::DataPointKey, std::any> result =  unordered_map<DataPoint::DataPointKey, std::any>(
                 {{DataPoint::SUBJECT_NAME, tmpTrigger->GetName()},
                 {DataPoint::SENSOR_KIND, string("temperature")},
                {DataPoint::SENSOR_BRAND, string("none")},
                {DataPoint::SENSOR_MODEL, string("generic")},
                {DataPoint::SENSOR_VALUE, 0.0},
                {DataPoint::SENSOR_UNIT, string("celsius")}}); 
        return result;   
 
    }
}
