#pragma once 

#include "base.h"

namespace twogreencows_core
{
    class timeline:public base {

        string name;
    public:
        virtual string getPrefix() const;
        string getName();

        timeline(string name);

    };
}





