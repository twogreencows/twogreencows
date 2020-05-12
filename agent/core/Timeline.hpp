#pragma once 

#include <vector>
#include "Base.hpp"
#include "Trigger.hpp"

namespace twogreencows_core
{
    class Timeline:public Base {
        vector<Trigger *> *AllTriggers; 
        string name;

    public:
        virtual string GetPrefix() const;
        string GetName();

        Timeline(string name);
        ~Timeline();
        void ScheduleTrigger(Trigger *trigger);
        std::vector<Event *> StartTriggers(int kq, long secondsInDay = -1);

         std::vector<Event*>  GetNextEventForTriggers(int kq, long secondsInDay = -1);
    private:
        void GetLastEventForTriggers(int kq, long secondsInDay = -1);
    };
}





