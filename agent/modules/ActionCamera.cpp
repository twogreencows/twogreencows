

#include "Base.hpp"
#include "Growbox.hpp"
#include "Action.hpp"
#include "ActionCamera.hpp"
#include  "DataPoint.hpp"
#include  "Trigger.hpp"
#include "Timeline.hpp"

using namespace std;


namespace twogreencows_core
{
    ActionCamera::ActionCamera(string growboxIdentifier): Action(growboxIdentifier)
    {
            Growbox *owningGrowBox = static_cast<twogreencows_core::Growbox*>(Base::ObjectWithIdentifier(this->GetOwnerIdentifier()));
            struct stat st = {0};
            this->PathToDataFolder= owningGrowBox->GetDataFolderPath()+"Data/"+this->GetIdentifier()+"/";
            if (stat((this->PathToDataFolder).c_str(), &st) == -1) {
                mkdir((this->PathToDataFolder).c_str(), 0744);
            }
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


        time_t DeviceDate = time(NULL);
        char buf[sizeof "2011-10-08T07:07:09Z"];
        strftime(buf, sizeof buf, "%FT%TZ", gmtime(&DeviceDate));
        string PathToPhoto = this->PathToDataFolder + string(buf);

#ifdef __MACH__
#else
        raspicamcontrol_set_defaults(&camera_parameters);
#endif

        unordered_map<DataPoint::DataPointKey, std::any> result =  unordered_map<DataPoint::DataPointKey, std::any>(
                 {{DataPoint::SUBJECT_NAME, tmpTrigger->GetName()},
                 {DataPoint::SENSOR_KIND, string("camera")},
                 {DataPoint::SENSOR_BRAND, string("none")},
                 {DataPoint::SENSOR_MODEL, string("generic")},
                 {DataPoint::SENSOR_ATTACHMENT, PathToPhoto}}); 
        return result;   
    }
}
