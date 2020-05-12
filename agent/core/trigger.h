#pragma once

#include "base.h"
#include "trigger.h"
#include "Action.h"

namespace twogreencows_core
{
    class trigger:public base {
        enum State{
            ON,
            OFF
        };


        trigger::State state;
        Action *action;
        bool transient;       
        string name;
        
    public:
        virtual string GetPrefix() const;
        trigger(string name, Action *action = 0);
        string GetName();
    };
}



