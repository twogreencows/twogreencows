
#include <string>
#include <iostream>
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
            this->timelines = new vector<timeline *>();

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

        void GrowBox::AddTimeline(timeline *tl)
        {
            timelines->push_back(tl);
        }
        
        void GrowBox::Start()
        {
            std::time(&start_date);
            stop_date = 0;
            isRunning = true;
            
            cout << "Starting growbox \" " << this->name << "\"("<< this->GetIdentifier() <<")" <<ctime(&start_date) << endl;
            
            struct tm * timeinfo;
            timeinfo = std::localtime(&start_date);
            int SecondsInDay = timeinfo->tm_hour * 3600 + timeinfo->tm_min *60 + timeinfo->tm_sec;
            cout << SecondsInDay << endl;

            kq = kqueue();

            struct kevent change;    /* event we want to monitor */
            struct kevent event;     /* event that was triggered */
            struct timespec tmout = { 10,     /* block for 5 seconds at most */ 
                          0 };
            
            
            
            EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 5000, 0);
            while(isRunning) {
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
