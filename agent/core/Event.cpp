#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Event.hpp" 
#include "Base.hpp"
#include "Timeline.hpp"
#include "Trigger.hpp"
#include <unordered_map>

using namespace std;


namespace twogreencows_core
{

    string Event::GetClassPrefix() const
    {
        return "event";
    }

    int Event::GetClassVersion() const
    {
        return 1;
    }


    Event::Event(long SecondsInDay, long Duration, string triggerIdentifier, string timelineIdentifier, Base::State state)
    {
        this->SetUpIdentifier();
        this->ObjectVersion = 1;
        this->duration = Duration;
        this->secondsInDay = SecondsInDay;
        this->triggerIdentifier = triggerIdentifier;
        this->timelineIdentifier = timelineIdentifier;
        this->state = state;
        this->relativeSecondsToNext = 0;
    }


    Event::Event(const Event &initialEvent)
    {
        
    }

    Event::~Event()
    {
        cout << "Destructir even" <<endl;
    }
    long Event::GetSecondsInDay() const
    {   
        return this->secondsInDay;
    }


    long Event::GetDuration() const
    {
        return this->duration;
    }

    long Event::GetRelativeSecondsToNext() const
    {
        return this->relativeSecondsToNext;
    }

    Event *Event::GetSibling()
    {
        return this->sibling;
    }

    string Event::GetTriggerIdentifier() const
    {   
        return this->triggerIdentifier;
    }

    string Event::GetTimelineIdentifier() const
    {   
        return this->timelineIdentifier;
    }

    void Event::SetSibling(Event *sibling) 
    {
        this->sibling = sibling;
    }

    void Event::SetTimelineIdentifier(string timelineIdentifier) 
    {  
        this->timelineIdentifier = timelineIdentifier;
    }

    Base::State Event::GetState() const
    {
        return this->state;
    }

    void Event::SetRelativeSecondsToNext(long relativeSecondsToNext)
    {
        this->relativeSecondsToNext = relativeSecondsToNext;
    }

}




std::ostream &operator<<(std::ostream &os, twogreencows_core:: Event const &e) { 
    twogreencows_core::Timeline *tl = static_cast<twogreencows_core::Timeline *> (twogreencows_core::Base::ObjectWithIdentifier(e.GetTimelineIdentifier()));
    twogreencows_core::Trigger *tr = static_cast<twogreencows_core::Trigger *> (twogreencows_core::Base::ObjectWithIdentifier(e.GetTriggerIdentifier()));
    
    bool isTransient = tr->GetTransient();
    long start = e.GetSecondsInDay();
    long start_m_s = start % 3600;
    long start_h = (start - start_m_s) /3600;
    long start_s = start_m_s % 60;
    long start_m =  (start_m_s - start_s) / 60;

    long length = e.GetDuration();
    long length_m_s = length % 3600;
    long length_h = (length - length_m_s) /3600;
    long length_s = length_m_s % 60;
    long length_m =  (length_m_s - length_s) / 60;

    string stateString = "ON ";
    if (e.GetState() == twogreencows_core::Base::OFF) {
        stateString = "OFF";
    }
    if (isTransient) {
     return os << "["+e.GetIdentifier(true) + "] - " << stateString << " at " 
        << std::setfill('0') << std::setw(2) << start_h <<"H" 
        << std::setfill('0') << std::setw(2) << start_m << "m" 
        << std::setfill('0')  << std::setw(2) << start_s << "s "  
        << "(" << e.GetSecondsInDay() <<")"    
        << " -- Transient -- " 
        << "next schedule in " << e.GetRelativeSecondsToNext() <<  ", "
        << "FOR timeline: "+ tl->GetName()  + "/trigger "+ tr->GetName();
} else {
    return os << "["+e.GetIdentifier(true) + "] - " << stateString << " at " 
        << std::setfill('0') << std::setw(2) << start_h <<"H" 
        << std::setfill('0') << std::setw(2) << start_m << "m" 
        << std::setfill('0')  << std::setw(2) << start_s << "s "  
        << "(" << e.GetSecondsInDay() <<")"    
        << " for " 
        <<  std::setfill('0') << std::setw(2) << length_h << "H:" 
        <<  std::setfill('0') << std::setw(2) <<length_m << "m" 
        <<  std::setfill('0') << std::setw(2) << length_s << "s "
        << "(" << e.GetDuration() << "), "    
        << "next schedule in " << e.GetRelativeSecondsToNext() <<  ", "
        << "FOR timeline: "+ tl->GetName()  + "/trigger "+ tr->GetName();
    }
}

