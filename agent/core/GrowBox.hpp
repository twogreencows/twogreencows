#pragma once

#include "Base.hpp"
#include "Timeline.hpp"
#include "Device.hpp"
#include "Recorder.hpp"
#include <ctime>
#include <vector>
#include <uv.h>
#include <sys/time.h>
#include <sys/types.h>
#ifdef __MACH__
#include <sys/event.h>
#endif

using namespace std;

namespace twogreencows_core
{
    class GrowBox: public Base {
        string name;
        string dataFolderPath;
        time_t start_date;
        time_t stop_date;

        //int kq;
        bool isRunning;
        vector<Timeline *> *timelines;
        vector<Recorder*> *recorders;
        Device *device;


        bool waitsForEvent;
        time_t  waitTime;
        vector<Event*> *nextEvents;

        uv_timer_t timer_req;
        uv_loop_t *loop;
     public:
        GrowBox(string name);
        virtual string GetPrefix() const;
        string GetName();

        static void signalHandler(int signum);
        void AddTimeline(Timeline *tl);
        void Start();
        void Stop();
        void HandleTimerCallback();
    };
}
