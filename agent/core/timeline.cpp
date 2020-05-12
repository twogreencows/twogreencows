#import <string>

#include <string>
#include <iostream>
#include "base.h"
#include "timeline.h" 
#include "trigger.h"

using namespace std;


namespace twogreencows_core
{

        string timeline::GetPrefix() const
        {
            return "timeline";
        }


       timeline::timeline(string name)
        {
            this->SetUpIdentifier();
            this->name = name;
            this->AllTriggers = new vector<trigger*>();
        }
       
       string timeline::GetName()
        {
            return this->name;
        }
        
        void timeline::ScheduleTrigger(trigger *trigger)
        {

        }

        void timeline::StartTriggers(int kq)
        {
        
        }
}

