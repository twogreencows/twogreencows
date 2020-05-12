
#pragma once

#include "Base.hpp"

using namespace std;

#include <map>
#include <vector>


namespace twogreencows_core
{
    class Event:public Base {

    private: 
        string triggerIdentifier;
        string timelineIdentifier;
        long secondsInDay;
        long relativeSecondsToNext;
        long duration;
        Base::State state;
        Event *sibling; 

    public:
        virtual string GetPrefix() const;
        long GetSecondsInDay() const;
        long GetDuration() const;
        long GetRelativeSecondsToNext() const;
        Event *GetSibling();
        string GetTriggerIdentifier() const;
        string GetTimelineIdentifier() const;

        Base::State GetState() const;

        void SetTimelineIdentifier(string timelineIdentifier);
        void SetRelativeSecondsToNext(long relativeSecondsToNext);
        void SetSibling(Event *sibling);

        Event(long timeInDay=-1, long duration = -1, string triggerIdentifier="", string timelineIdentifier="", Base::State state=Base::ON );
        
        Event(const Event &initialEvent);
        ~Event();
    };
}


std::ostream &operator<<(std::ostream &os, twogreencows_core::Event const &e); 


