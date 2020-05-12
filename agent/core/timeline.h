#pragma once 

#include "base.h"

namespace twogreencows_core
{
    class timeline:public base {

        string name;
    public:
        virtual string GetPrefix() const;
        string GetName();

        timeline(string name);

    };
}





