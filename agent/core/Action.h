#pragma once

#include "base.h"

using namespace std;


namespace twogreencows_core
{
    class Action:base{
            string identifier;
        public:
        
        //Action();
        virtual void Execute();
        virtual string GetPrefix() const;
    };
}
