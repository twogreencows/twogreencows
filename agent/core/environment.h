#pragma once

#include "base.h"
#include "timeline.h"
#include <ctime>
#include <vector>

using namespace std;

namespace twogreencows_core
{
    class environment: public base {
        string name;
        time_t start_date;

    public:
      environment(string name);
      virtual string GetPrefix() const;
      string GetName();

      void AddTimeline(timeline tl);
      void Start();

    };
}
