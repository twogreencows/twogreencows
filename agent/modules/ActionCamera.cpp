

#include "Base.hpp"
#include "Growbox.hpp"
#include "Action.hpp"
#include "ActionCamera.hpp"
#include "DataPoint.hpp"
#include "Trigger.hpp"
#include "Timeline.hpp"

#ifdef __arm__
#include <mmal.h>
#endif

using namespace std;


namespace twogreencows_core
{
    ActionCamera::ActionCamera(string growboxIdentifier): Action(growboxIdentifier)
    {
            Growbox *owningGrowBox = static_cast<twogreencows_core::Growbox*>(Base::ObjectWithIdentifier(this->GetOwnerIdentifier()));
            struct stat st = {0};
            string PathToGlobalDataFolder = owningGrowBox->GetDataFolderPath()+"Data/";

            if (stat((PathToGlobalDataFolder).c_str(), &st) == -1) {
                mkdir((PathToGlobalDataFolder).c_str(), 0744);
            }
            this->PathToDataFolder= owningGrowBox->GetDataFolderPath()+"Data/"+this->GetIdentifier()+"/";
            if (stat((this->PathToDataFolder).c_str(), &st) == -1) {
                mkdir((this->PathToDataFolder).c_str(), 0744);
            }

#ifdef __arm__
            MMAL_COMPONENT_T *camera_info;
            MMAL_STATUS_T status = MMAL_SUCCESS ;
            MMAL_PARAMETER_CAMERA_INFO_T param;
            param.hdr.id = MMAL_PARAMETER_CAMERA_INFO;
            param.hdr.size = sizeof(param)-4;  // Deliberately undersize to check firmware veresion

            status = mmal_port_parameter_get(camera_info->control, &param.hdr);
            /*mmal_port_parameter_get(camera_info->control, &param.hdr);
            char camera_name[MMAL_PARAMETER_CAMERA_INFO_MAX_STR_LEN]
            status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA_INFO, &camera_info);
            if (status == MMAL_SUCCESS)
            {
                MMAL_PARAMETER_CAMERA_INFO_T param;
                param.hdr.id = MMAL_PARAMETER_CAMERA_INFO;
                param.hdr.size = sizeof(param)-4;  // Deliberately undersize to check firmware veresion
                status = mmal_port_parameter_get(camera_info->control, &param.hdr);
                if (status != MMAL_SUCCESS)
                {
                    param.hdr.size = sizeof(param);
                    status = mmal_port_parameter_get(camera_info->control, &param.hdr);
                    if (status == MMAL_SUCCESS && param.num_cameras > 0)
                    {
            // Take the parameters from the first camera listed.
                        cerr << param.cameras[0].max_width << endl;
                        cerr << param.cameras[0].max_height << endl;
                        cerr << param.cameras[0].camera_name << endl;
                    }
                }
            }*/
#endif 
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
        cerr << "Taking picture at " << this->width << this->height << this->quality <<endl;
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
