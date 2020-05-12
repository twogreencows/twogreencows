
#include <string>
#include <iostream>
#include <iomanip>
#include "GrowBox.hpp" 
#include "Base.hpp"

#include <csignal>

using namespace std;


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

        signal(SIGINT, GrowBox::signalHandler); 
        signal(SIGILL, GrowBox::signalHandler); 
        signal(SIGTERM, GrowBox::signalHandler); 

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

        kq = kqueue();
        vector<Event*> *nextEvents = new vector<Event*>();

        cout << "Start Timeline triggers " << endl;

        for(std::vector<Timeline *>::iterator itt = this->timelines->begin(); itt != this->timelines->end(); ++itt) {
            std::vector<Event *> triggerEvents =  (*itt)->StartTriggers(kq, SecondsInDay);
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

        struct kevent change;    /* event we want to monitor */
        struct kevent event;     /* event that was triggered */
        struct timespec tmout = { 86400*1000,     /* block for 5 seconds at most */ 
            0 };

        int waitsForEvent = true;
        int waitTime =  nextEvents->at(0)->GetRelativeSecondsToNext();

        cout << " == Will wake up in " << waitTime << endl;
        EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, waitTime*1000, 0);

        while(isRunning) {
            int nev = kevent(kq, &change, 1, &event, 1, &tmout);
            if (nev > 0) {

                time_t currentTime = 0;
                struct tm *currentTimeInfo;
                std::time(&currentTime);   
                currentTimeInfo = std::localtime(&currentTime);
                long SecondsInDay = currentTimeInfo->tm_hour * 3600 + currentTimeInfo->tm_min *60 + currentTimeInfo->tm_sec;

                cout << " == Received event count of  " << nev << " at " << SecondsInDay <<endl;
                if (true == waitsForEvent ) {

                    cout << " == Fire ot triggers " <<  endl;
                    //trigger the Fire

                    for(std::vector<Event *>::iterator it = nextEvents->begin(); it != nextEvents->end(); ++it) {
                        if ((*it)->GetRelativeSecondsToNext() == waitTime) {
                            Trigger *tr = static_cast<Trigger *>(Base::ObjectWithIdentifier((*it)->GetTriggerIdentifier()));
                            cout << tr->GetName();
                            tr->Fire(*it, SecondsInDay);
                        } 
                        (*it)->SetRelativeSecondsToNext(0);
                    }
                    //wait for one second to avoid looping on the action
                    waitsForEvent = false;
                    cout << " == Schedule for 2 second " <<  endl;
                    EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 2000, 0);
                }  else {

                    cout << " == Schedule next events" <<  endl;
                    waitsForEvent = true ;

                    nextEvents->clear();

                    for(std::vector<Timeline *>::iterator itt = this->timelines->begin(); itt != this->timelines->end(); ++itt) {
                        std::vector<Event *> triggerEvents =  (*itt)->GetNextEventForTriggers(kq, SecondsInDay);
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


                    waitTime = nextEvents->at(0)->GetRelativeSecondsToNext();
                    cout << " == Will wake up in " << waitTime << endl;
                    EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, waitTime*1000, 0);
                }
            } else {
                cout <<"no kevent" << endl;
            }
        }
        delete  nextEvents;
    }

    void GrowBox::Stop()
    {
        this->stop_date = std::time(0);
        //close(kq)
    }

}
