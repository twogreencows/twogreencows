#include <string>
#include <iostream>
#include "Event.hpp" 
#include "Base.hpp"


using namespace std;


namespace twogreencows_core
{

        string Event::GetPrefix() const
        {
            return "event";
        }


        Event::Event(int SecondsInDay, string triggerIdentifier, string timeLineIdentifier)
        {
            this->secondsInDay = SecondsInDay;
            this->triggerIdentifier = triggerIdentifier;
            this->timeLineIdentifier = timeLineIdentifier;
        }
        
}
 
