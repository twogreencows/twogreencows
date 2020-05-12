
#include <string>
#include <iostream>
#include "trigger.h"
#include "Action.h"

using namespace std;


namespace twogreencows_core
{

        string trigger::GetPrefix() const
        {
            return "trigger";
        }


        trigger::trigger(string name, Action *action , vector<pair<time_t, trigger::State> > sequence )
        {
            this->SetUpIdentifier();
            this->name = name;
            this->sequence = new map<int, trigger::State>();
                
                //fill the sequence with incremental date
            int tmpInt = 0;
            for(std::vector<pair<time_t, trigger::State> >::iterator it = sequence.begin(); it != sequence.end(); ++it) {
                tmpInt += (*it).first;
                this->sequence->insert( pair<int, trigger::State> (tmpInt, (*it).second));
            }
        }
        

        trigger::~trigger()
        {
            delete(this->sequence);
        }

        string trigger::GetName()
        {
            return(this->name);
        }

        bool trigger::GetTransient() 
        {
            return(this->transient);
        }

        void trigger::SetTransient(bool newValue)
        {
            this->transient = newValue;
        }

        void trigger::Execute(int currentTime, string  TimelineUUID)
        {
            bool shouldExecute = true;
            if (this->activeStartDate != 0) {

            }

            if(this->transient) 
            {
                this->state = trigger::OFF;
            }
        }
        
}
