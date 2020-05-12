#include "Base.hpp"
#include "Action.hpp"
#include "ActionCamera.hpp"
#include  "DataPoint.hpp"
#include  "Trigger.hpp"
#include "Timeline.hpp"

using namespace std;


namespace twogreencows_core
{
    ActionCamera::ActionCamera()
    {
    }
    
        
    ActionCamera::~ActionCamera()
    {
    }

    string ActionCamera::GetType() const
    {
        return "camera";
    }

    unordered_map<DataPoint::DataPointKey, std::any> ActionCamera::Execute(string TriggerUUID)
    {
        Trigger *tmpTrigger = static_cast<Trigger *> (Base::ObjectWithIdentifier(TriggerUUID));

        unordered_map<DataPoint::DataPointKey, std::any> result =  unordered_map<DataPoint::DataPointKey, std::any>(
                 {{DataPoint::SUBJECT_NAME, tmpTrigger->GetName()},
                 {DataPoint::SENSOR_KIND, string("camera")},
                 {DataPoint::SENSOR_BRAND, string("none")},
                 {DataPoint::SENSOR_MODEL, string("generic")},
                 {DataPoint::SENSOR_ATTACHMENT, string("/path/to/photo")}}); 
        return result;   
    }
}
