#pragma once

#include "Action.hpp"


#ifdef __MACH__ 
#else 
#include "interface/mmal/mmal.h"
#include "interface/mmal/mmal_logging.h"
#include "interface/mmal/mmal_buffer.h"
#include "interface/mmal/util/mmal_util.h"
#include "interface/mmal/util/mmal_util_params.h"
#include "interface/mmal/util/mmal_default_components.h"
#include "interface/mmal/util/mmal_connection.h"
#include "interface/mmal/mmal_parameters_camera.h"

#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_CAPTURE_PORT 2

#endif


using namespace std;


namespace twogreencows_core
{
    class ActionCamera:public Action{
            string identifier;
            string PathToDataFolder;
            int width = 320;//2592;
            int height = 200; //1944;
            int quality = 90;
#ifdef __MACH__
#else            
            //RASPICAM_CAMERA_PARAMETERS camera_parameters;
#endif
    public:
        
        ActionCamera(string growboxIdentifier);
        ~ActionCamera();
        virtual string GetType() const;
        unordered_map<DataPoint::DataPointKey, std::any> Execute(string TriggerUUID);
    };
}
