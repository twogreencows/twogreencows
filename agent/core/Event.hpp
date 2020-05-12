
#pragma once

#include "Base.hpp"

using namespace std;

#include <map>
#include <vector>


namespace twogreencows_core
{
    class Event:public Base {

    private: 
        string triggerIdentifier;
        string timeLineIdentifier;
        int secondsInDay;


    public:
        virtual string GetPrefix() const;
        Event(int hInDay=-1, string triggerIdentifier="", string timeLineIdentifier="");
    };
}


