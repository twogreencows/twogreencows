#pragma once

#include <string>
#include "Base.hpp"

using namespace std;

namespace twogreencows_core
{
    class Device: public Base {
        string name;


    public:
      Device(string name = "");
      virtual string GetPrefix() const;
      string GetName();
    };
}
