#import <string>

#include <string>
#include <iostream>
#include "GrowBox.hpp" 
#include "base.h"

#include <csignal>

using namespace std;


namespace twogreencows_core
{

        string GrowBox::GetPrefix() const
        {
            return "GrowBox";
        }


        GrowBox::GrowBox(string name)
        {
            this->SetUpIdentifier();
            this->name = name;
            this->timelines = new vector<timeline *>();

            signal(SIGINT, GrowBox::signalHandler); 
            signal(SIGILL, GrowBox::signalHandler); 
            signal(SIGTERM, GrowBox::signalHandler); 
    
        }
        
        void GrowBox::signalHandler(int signum)
        {
            cout << "signal" << signum <<endl;
        }
        
        string GrowBox::GetName()
        {
            return(this->name);
        }

        void GrowBox::AddTimeline(timeline *tl)
        {
            timelines->push_back(tl);
        }
        
        void GrowBox::Start()
        {
            start_date = std::time(0);
            stop_date = 0;
            kq = kqueue();
            cout << kq;
            struct kevent change;    /* event we want to monitor */
            struct kevent event;     /* event that was triggered */
           struct timespec tmout = { 10,     /* block for 5 seconds at most */ 
                          0 };
            cout << " Start" << endl;
            EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 5000, 0);
            for (;;) {
                int nev = kevent(kq, &change, 1, &event, 1, &tmout);
                if (nev > 0) {
                    cout <<"in kevent" << endl;
                }
                cout <<"no kevent";
            }
        }

        void GrowBox::Stop()
        {
            this->stop_date = std::time(0);
            //close(kq)
        }

}
