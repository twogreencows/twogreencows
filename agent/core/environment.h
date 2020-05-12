#pragma once

#include "base.h"
#include "timeline.h"
#include <ctime>
#include <vector>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/event.h>

using namespace std;

namespace twogreencows_core
{
    class environment: public base {
        string name;
        time_t start_date;
        time_t stop_date;

        int kq;

        vector<timeline *> *timelines;

    public:
      environment(string name);
      virtual string GetPrefix() const;
      string GetName();

      static void signalHandler(int signum);
      void AddTimeline(timeline *tl);
      void Start();
      void Stop();

    };
}
