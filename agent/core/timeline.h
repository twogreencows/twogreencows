#pragma once 

#include <vector>
#include "base.h"
#include "trigger.h"

namespace twogreencows_core
{
    class timeline:public base {
        vector<trigger *> *AllTriggers; 
        string name;
    public:
        virtual string GetPrefix() const;
        string GetName();

        timeline(string name);
        void ScheduleTrigger(trigger *trigger);
        void StartTriggers(int kq);

    };
}





