#pragma once

#include "Base.hpp"
#include "Action.h"
#include "Event.hpp"
#include <climits>

using namespace std;

#include <map>
#include <vector>


namespace twogreencows_core
{
    class Trigger:public Base {


    private: 
        string name;
        Base::State state;
        Action *action;
        string ownerIdentifier="";
        bool transient; //means that after each action it will default to OFF. 
#ifdef __MACH__
        time_t activeStartDate = ULLONG_MAX;
        time_t activeEndDate = ULLONG_MAX;
#else 
        time_t activeStartDate = LONG_MAX;
        time_t activeEndDate = LONG_MAX;
#endif
        int dayPeriod = 1;
        vector<Event*> *sequence = NULL;

    public:
            //Life cycle
        Trigger(string name, Action *action = 0, vector<pair<time_t, Base::State> > sequence = { {86400, Base::ON} }, bool transient=false);
        ~Trigger();
        
        virtual string GetPrefix() const;
        string GetName();
        bool GetTransient();
        Base::State GetState() const;
        string GetOwnerIdentifier();
        void SetState(Base::State newState);
        void SetOwnerIdentifier(string ownerIdentifier);

        void SetTransient(bool newValue);
        Event *GetNextEventForSecondsInDay(long SecondsInDay);
        Event *GetLastEventForSecondsInDay(long SecondsInDay);
        void Fire(Event *event, int currentTime);
    };
}



