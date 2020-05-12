#pragma once 

#include <vector>
#include "Base.hpp"
#include <sqlite3.h>
#include "Trigger.hpp"

namespace twogreencows_core
{
    class Timeline:public Base {
        vector<Trigger *> *AllTriggers; 
        string name;

    public:
        virtual string GetClassPrefix() const;
        string GetName();
        string GetOwnerIdentifier();
        void SetOwnerIdentifier(string ownerIdentifier);
        virtual int GetClassVersion() const;

        Timeline(string name="Timeline", std::string identifier="");
        ~Timeline();
        void ScheduleTrigger(Trigger *trigger);
        bool PersistToStorage(sqlite3 *db);
        std::vector<Event *> StartTriggers(int kq, long secondsInDay = -1);

        std::vector<Event*>  GetNextEventForTriggers(int kq, long secondsInDay = -1);
    private:
        void GetLastEventForTriggers(int kq, long secondsInDay = -1);
        string ownerIdentifier="";
    };
}





