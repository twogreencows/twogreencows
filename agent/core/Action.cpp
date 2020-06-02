
#include <string>
#include <iostream>
#include <iomanip>

#include "Base.hpp"
#include "Action.hpp"
#include "Trigger.hpp"
#include "Timeline.hpp"

#include "ActionTemperatureSensor.hpp"
#include "ActionRelay.hpp"
#include "ActionCamera.hpp"

using namespace std;


namespace twogreencows_core
{

        string Action::GetClassPrefix() const
        {
            return "action";
        }

        int Action::GetClassVersion() const 
        {
            return 1;
        }

        Action::Action(string GrowboxIdentifier)
        {
            this->SetUpIdentifier();
            this->ObjectVersion = 1; 
            this->OwnerIdentifier = GrowboxIdentifier;
        }
        
        Action::~Action()
        {
            cout << "Destructor Action" << endl;
        }

       
        string Action::GetOwnerIdentifier()
        {
            return this->OwnerIdentifier;
        }

        unordered_map<DataPoint::DataPointKey, std::any> Action::Execute(string TriggerUUID)
        {
            Trigger *tmpTrigger = static_cast<Trigger *> (Base::ObjectWithIdentifier(TriggerUUID));
            Timeline *tl = static_cast<Timeline*> (Base::ObjectWithIdentifier(tmpTrigger->GetOwnerIdentifier()));
            string stateString = (tmpTrigger->GetState() == Base::ON) ? "ON" : "OFF";

         unordered_map<DataPoint::DataPointKey, std::any> result =  unordered_map<DataPoint::DataPointKey, std::any>(
                 {{DataPoint::SUBJECT_NAME , tmpTrigger->GetName()},
                 {DataPoint::SENSOR_KIND, "generic"},
                {DataPoint::SENSOR_BRAND, "none"},
                {DataPoint::SENSOR_MODEL, "generic"},
                {DataPoint::SENSOR_VALUE, 0.0},
                {DataPoint::SENSOR_UNIT, "none"}}); 
            time_t raw_time =   time (NULL);

            
            cout << " [Action:"<< std::put_time(localtime(&raw_time), "%c") 
                 <<  "] " + tmpTrigger->GetIdentifier(true) 
                 << "(" + tl->GetName() << ", " <<tmpTrigger->GetName() + "): "<< stateString << endl;
                 return result;
        }

        Action * Action::CreateActionForType(string actionType, string growboxIdentifier) {
            cout << actionType << endl;
            Action *result = NULL;
            if (0 == actionType.compare("relay")) {
                result = new ActionRelay(growboxIdentifier);
            } else  if (0 == actionType.compare("camera")) {
                result = new ActionCamera(growboxIdentifier);
            } else  if (0 == actionType.compare("temperature")) {
                result = new ActionTemperatureSensor(growboxIdentifier);
            } else {
                cerr << "NO action found " << actionType << endl;
                result = nullptr;

            }
            return result;
        }



}
