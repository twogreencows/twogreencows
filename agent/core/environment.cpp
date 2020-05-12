#import <string>

#include <string>
#include <iostream>
#include "environment.h" 
#include "base.h"

#include <csignal>

using namespace std;


namespace twogreencows_core
{

        string environment::GetPrefix() const
        {
            return "environment";
        }


        environment::environment(string name)
        {
            this->SetUpIdentifier();
            this->name = name;
            this->timelines = new vector<timeline *>();

            signal(SIGINT, environment::signalHandler); 
            signal(SIGILL, environment::signalHandler); 
            signal(SIGTERM, environment::signalHandler); 
    
        }
        
        void environment::signalHandler(int signum)
        {
            cout << "signal" << signum <<endl;
        }
        
        string environment::GetName()
        {
            return(this->name);
        }

        void environment::AddTimeline(timeline *tl)
        {
            timelines->push_back(tl);
        }
        
        void environment::Start()
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

        void environment::Stop()
        {
            this->stop_date = std::time(0);
            //close(kq)
        }

}
