
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <iomanip>
#include "GrowBox.hpp" 
#include "Base.hpp"
#include <pwd.h>
#include <unistd.h>
#include <csignal>
#include <uv.h>
#include <algorithm>

using namespace std;


void TimelinesMainCallBack(uv_timer_t* handle);

void TimelinesMainCallBack(uv_timer_t* handle)
{
    void *data = uv_handle_get_data((uv_handle_t *)handle);
    twogreencows_core::GrowBox *tmpGrowbox = static_cast<twogreencows_core::GrowBox *>(data);
    cout << "================= RECEIVING FOR : " << tmpGrowbox->GetIdentifier(true) << endl;
    tmpGrowbox->HandleTimerCallback();
}



namespace twogreencows_core
{

    string GrowBox::GetPrefix() const
    {
        return "growBox";
    }


    GrowBox::GrowBox(string name)
    {
        this->SetUpIdentifier();
        this->name = name;
        this->timelines = new vector<Timeline *>();
        this->recorders= new vector<Recorder *>();
        this->device = new Device();

        signal(SIGINT, GrowBox::signalHandler); 
        signal(SIGILL, GrowBox::signalHandler); 
        signal(SIGTERM, GrowBox::signalHandler); 

        string homedir = getenv("HOME");
        if (homedir.empty() ) {
            struct passwd *pw = getpwuid(getuid());
            homedir = pw->pw_dir;
        }
        this->dataFolderPath =homedir + "/.twogreencows/" + this->GetIdentifier()+"/" ;
        string twogreencowsMainFolder = homedir + "/.twogreencows/";
        cout << this->dataFolderPath << endl;
        struct stat st = {0};

        if (stat(twogreencowsMainFolder.c_str(), &st) == -1) {
            mkdir(twogreencowsMainFolder.c_str(), 0700);
        }

        if (stat(this->dataFolderPath.c_str(), &st) == -1) {
            mkdir(this->dataFolderPath.c_str(), 0700);
        }
    }

    void GrowBox::signalHandler(int signum)
    {
        exit(0);
    }

    string GrowBox::GetName()
    {
        return(this->name);
    }

    void GrowBox::AddTimeline(Timeline *tl)
    {
        timelines->push_back(tl);
    }

    void GrowBox::Start()
    {
        std::time(&start_date);
        stop_date = 0;
        isRunning = true;


        struct tm * timeinfo;
        timeinfo = std::localtime(&start_date);
        long SecondsInDay = timeinfo->tm_hour * 3600 + timeinfo->tm_min *60 + timeinfo->tm_sec;

        cout << "Starting growbox \" " << this->name << "\"("<< this->GetIdentifier(true) <<")" << ctime(&start_date);
        long start = SecondsInDay;
        long start_m_s = start % 3600;
        long start_h = (start - start_m_s) /3600;
        long start_s = start_m_s % 60;
        long start_m =  (start_m_s - start_s) / 60;

        cout << "  + Time is at " << std::dec <<SecondsInDay << " in day " 
            <<  std::setfill('0') << std::setw(2)<< start_h <<"H:"
            << std::setfill('0') << std::setw(2)<< start_m << "m:" 
            <<std::setfill('0') << std::setw(2)<< start_s << "s " <<endl;

        this->nextEvents = new vector<Event*>();

        cout << "Start Timeline triggers " << endl;

        for(std::vector<Timeline *>::iterator itt = this->timelines->begin(); itt != this->timelines->end(); ++itt) {
            std::vector<Event *> triggerEvents =  (*itt)->StartTriggers(0, SecondsInDay);
            for (std::vector<Event *>::iterator ite = triggerEvents.begin(); ite != triggerEvents.end(); ite++) {
                this->nextEvents->push_back(*ite);
            }
        }

        std::sort(this->nextEvents->begin(), this->nextEvents->end(), [](Event* a, Event * b) {
                return a->GetRelativeSecondsToNext() < b->GetRelativeSecondsToNext(); });

        cout << " == Dumping next events " << endl;
        for(std::vector<Event *>::iterator it = this->nextEvents->begin(); it != this->nextEvents->end(); ++it) {
            cout << *(*it) <<endl;
        }

        //struct kevent change;    /* event we want to monitor */
        //struct kevent event;     /* event that was triggered */
        //struct timespec tmout = { 86400*1000,     /* block for 5 seconds at most */ 
        //    0 };

        this->waitsForEvent = true;
        this->waitTime =  nextEvents->at(0)->GetRelativeSecondsToNext();

        cout << " == Will wake up in " << this->waitTime << endl;
        //EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, waitTime*1000, 0);h

        this->loop = uv_default_loop();
        uv_timer_init(loop, &(this->timer_req));
        uv_handle_set_data((uv_handle_t *) &(this->timer_req), static_cast<void *>(this));

        uv_timer_start(&(this->timer_req), TimelinesMainCallBack, this->waitTime*1000, 0);
        uv_run(this->loop, UV_RUN_DEFAULT);        

        delete  this->nextEvents;

    }

    void GrowBox::HandleTimerCallback()
    {

        time_t currentTime = 0;
        struct tm *currentTimeInfo;
        std::time(&currentTime);   
        currentTimeInfo = std::localtime(&currentTime);
        long SecondsInDay = currentTimeInfo->tm_hour * 3600 + currentTimeInfo->tm_min *60 + currentTimeInfo->tm_sec;

        cout << " == Received timer " << " at " << SecondsInDay <<endl;
        if (true == this->waitsForEvent ) {

            cout << " == Fire ot triggers " <<  endl;
            //trigger the Fire

            for(std::vector<Event *>::iterator it = nextEvents->begin(); it != nextEvents->end(); ++it) {
                if ((*it)->GetRelativeSecondsToNext() == this->waitTime) {
                    Trigger *tr = static_cast<Trigger *>(Base::ObjectWithIdentifier((*it)->GetTriggerIdentifier()));
                    cout << tr->GetName();
                    tr->Fire(*it, SecondsInDay);
                } 
                (*it)->SetRelativeSecondsToNext(0);
            }
            //wait for one second to avoid looping on the action
            this->waitsForEvent = false;
            cout << " == Schedule for 2 second " <<  endl;
            uv_timer_stop(&(this->timer_req));
            uv_timer_start(&(this->timer_req), TimelinesMainCallBack, 2000, 0);

            //EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 2000, 0);
        }  else {

            cout << " == Schedule next events" <<  endl;
            this->waitsForEvent = true ;

            this->nextEvents->clear();

            for(std::vector<Timeline *>::iterator itt = this->timelines->begin(); itt != this->timelines->end(); ++itt) {
                std::vector<Event *> triggerEvents =  (*itt)->GetNextEventForTriggers(0, SecondsInDay);
                for (std::vector<Event *>::iterator ite = triggerEvents.begin(); ite != triggerEvents.end(); ite++) {
                    nextEvents->push_back(*ite);
                }
            }


            std::sort(nextEvents->begin(), nextEvents->end(), [](Event* a, Event * b) {
                    return a->GetRelativeSecondsToNext() < b->GetRelativeSecondsToNext(); });
            cout << " == Dumping next events " << endl;
            for(std::vector<Event *>::iterator it = nextEvents->begin(); it != nextEvents->end(); ++it) {
                cout << *(*it) <<endl;
            }


            this->waitTime = nextEvents->at(0)->GetRelativeSecondsToNext();

            cout << " == Will wake up in " << this->waitTime << endl;
            uv_timer_stop(&(this->timer_req));
            uv_timer_start(&(this->timer_req), TimelinesMainCallBack, this->waitTime*1000, 0);
            //EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, waitTime*1000, 0);
        }

    }
    void GrowBox::Stop()
    {
        this->stop_date = std::time(0);
        //close(kq)
    }

}
