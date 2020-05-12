#pragma once

#include "Base.hpp"

namespace twogreencows_core
{
    class Recorder:public Base {

    public:
        virtual string GetPrefix() const;
        Recorder();
    };

}
