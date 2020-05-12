#pragma once

#include <string>
#include "Base.hpp"

using namespace std;

namespace twogreencows_core
{
    class Hardware: public Base {
        string name;


    public:
      Hardware(string name);
      virtual string GetPrefix() const;
      string GetName();
    };
}
