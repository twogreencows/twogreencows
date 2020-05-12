#pragma once

#include "Base.hpp"
#include "DataPoint.hpp"
#include "Timeline.hpp"
#include "Device.hpp"
#include "Recorder.hpp"
#include <ctime>
#include <sqlite3.h>
#include <vector>
#include <unordered_map>
#include <uv.h>
#include <sys/time.h>
#include <sys/types.h>
#ifdef __MACH__
#include <sys/event.h>
#endif

using namespace std;

namespace twogreencows_core
{
    class Growbox: public Base {
        string name;
        string serverIPV4;
        string dataFolderPath;
        time_t start_date;
        time_t stop_date;

        sqlite3 *persistentStorage;
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
        //Growbox(string name, string serverIPAddress = "0.0.0.0");
        Growbox(unordered_map<std::string, std::string> parameters);
        virtual string GetClassPrefix() const;
        string GetName();
        string GetDataFolderPath();

        virtual int GetClassVersion() const;
        static void signalHandler(int signum);
        void AddTimeline(Timeline *tl);
        void LogDataPoint(DataPoint dp);
        void Start();
        void Stop();
        void HandleTimerCallback();
        bool PersistToStorage(sqlite3 *db);
    };
}
