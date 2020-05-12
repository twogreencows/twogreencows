
#include <string>
#include <iostream>
#include <iomanip>

#include "Base.hpp"
#include "Action.h"
#include "Trigger.hpp"
#include "Timeline.hpp"

using namespace std;


namespace twogreencows_core
{

        string Action::GetPrefix() const
        {
            return "action";
        }


        Action::Action()
        {
            this->SetUpIdentifier();
        }
        
        Action::~Action()
        {
            cout << "Destructor Action" << endl;
        }

        void Action::Execute(string TriggerUUID)
        {
            time_t raw_time =   time (NULL);

            Trigger *tmpTrigger = static_cast<Trigger *> (Base::ObjectWithIdentifier(TriggerUUID));
            Timeline *tl = static_cast<Timeline*> (Base::ObjectWithIdentifier(tmpTrigger->GetOwnerIdentifier()));
            string stateString = (tmpTrigger->GetState() == Base::ON) ? "ON" : "OFF";
            
            cout << " [Action:"<< std::put_time(localtime(&raw_time), "%c") 
                 <<  "] " + tmpTrigger->GetIdentifier(true) 
                 << "(" + tl->GetName() << ", " <<tmpTrigger->GetName() + "): "<< stateString << endl;
        }
}
