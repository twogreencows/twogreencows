
#include <string>
#include <iostream>
#include "Base.hpp"
#include "Timeline.hpp" 
#include "Trigger.hpp"

using namespace std;


namespace twogreencows_core
{

        string Timeline::GetPrefix() const
        {
            return "timeline";
        }


       Timeline::Timeline(string name)
        {
            this->SetUpIdentifier();
            this->name = name;
            this->AllTriggers = new vector<Trigger*>();
        }
       
       Timeline::~Timeline()
       {
           cout << "timeline destructor";
       }

       string Timeline::GetName()
        {
            return this->name;
        }
        
        void Timeline::ScheduleTrigger(Trigger *trigger)
        {
            trigger->SetOwnerIdentifier(this->GetIdentifier());

            AllTriggers->push_back(trigger);
            
            //cout <<"   -->  Now we have " << AllTriggers->size() << endl;
        }

        std::vector<Event*> Timeline::StartTriggers(int kq, long timeInSeconds)
        {
            //Build Events for day
            cout << "  + Starting timeline " << this->GetName() << "(" << this->GetIdentifier(true) << ")" << endl;
            //cout << this->AllTriggers->size() << " !!!" << endl;
            std::vector<Event *> result ; 
            //set trigger to the state they should havee if not transient
            for(std::vector<Trigger *>::iterator it = this->AllTriggers->begin(); it != this->AllTriggers->end(); ++it) {
                Event *triggerNextEvent = (*it)->GetNextEventForSecondsInDay(timeInSeconds);
                Event *triggerLastEvent = (*it)->GetLastEventForSecondsInDay(timeInSeconds);
                if ( (triggerLastEvent != NULL) && ((*it)->GetTransient() == false) ) {
                    (*it)->Fire(triggerLastEvent, timeInSeconds);
                }
                result.push_back(triggerNextEvent);
            }
            return result;
        }

        std::vector<Event *> Timeline::GetNextEventForTriggers(int kq, long timeInSeconds)
        {
            std::vector<Event *> result; 
            for(std::vector<Trigger *>::iterator it = this->AllTriggers->begin(); it != this->AllTriggers->end(); ++it) {
                Event *triggerNextEvent = (*it)->GetNextEventForSecondsInDay(timeInSeconds);
                result.push_back(triggerNextEvent);
            }
            return result;
        }
}

