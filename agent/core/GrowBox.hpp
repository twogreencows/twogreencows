#pragma once

#include "Base.hpp"
#include "Timeline.hpp"
#include "Hardware.hpp"
#include "Recorder.hpp"
#include <ctime>
#include <vector>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/event.h>

using namespace std;

namespace twogreencows_core
{
    class GrowBox: public Base {
        string name;
        string dataFolderPath;
        time_t start_date;
        time_t stop_date;

        int kq;
        bool isRunning;
        vector<Timeline *> *timelines;
        vector<Recorder*> *recorders;
        Hardware *device;

    public:
      GrowBox(string name);
      virtual string GetPrefix() const;
      string GetName();

      static void signalHandler(int signum);
      void AddTimeline(Timeline *tl);
      void Start();
      void Stop();

    };
}
