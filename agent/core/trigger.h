#pragma once

#include "Base.hpp"
#include "trigger.h"
#include "Action.h"

using namespace std;

#include <map>
#include <vector>


namespace twogreencows_core
{
    class trigger:public Base {

    public:
        enum State{
            ON,
            OFF
        };

    private: 
        string name;
        trigger::State state;
        Action *action;
        bool transient; //means that after each action it will default to OFF. 
        time_t activeStartDate = 0; 
        time_t activeEndDate = 0;
        int dayPeriod = 1;
        map<int, trigger::State> *sequence = NULL;

    public:
        virtual string GetPrefix() const;
        trigger(string name, Action *action = 0, vector<pair<time_t, trigger::State> > sequence = { {86400, trigger::ON} });
        ~trigger();
        string GetName();
        bool GetTransient();
        void SetTransient(bool newValue);
        void Execute(int currentTime, string TimelineUUID);
    };
}



