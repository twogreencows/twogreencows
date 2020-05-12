
#include <string>
#include <iostream>
#include "Trigger.hpp"
#include "Base.hpp"
#include "Action.h"

using namespace std;


namespace twogreencows_core
{

    string Trigger::GetPrefix() const
    {
        return "trigger";
    }


    Trigger::Trigger(string name, Action *action , vector<pair<time_t, Base::State> > sequence , bool transient)
    {
        this->SetUpIdentifier();
        this->name = name;
        this->transient = transient;
        this->sequence = new vector<Event* >();
        this->action = action;
        cout << "Create trigger: " << this->name << endl;

        //validate sequence: it has to cover 24 hours in case of
        long tmpLong = 0;
        bool hasOFFState = false;
        for(std::vector<pair<time_t, Base::State> >::iterator it = sequence.begin(); it != sequence.end(); ++it) {
            tmpLong += (*it).first;
            if (!hasOFFState) {
                hasOFFState = ((*it).second == Base::OFF);
            }
        }
        if (this->transient == false) {
            if (tmpLong != 86400) {
                Base::DumpSequence(sequence, transient);
                throw std::invalid_argument("Sequence for non transient trigger must cover 24 hours");
            }
        } else {
            if (hasOFFState) {
                cout << "WARNING: for transient trigger, OFF part will be ignored. A sequence should be a suite of ON state";
            }
        }

        //fill the sequence with incremental date
        tmpLong = 0;
        for(std::vector<pair<time_t, Base::State> >::iterator it = sequence.begin(); it != sequence.end(); ++it) {
            Event *tmpEvent = new Event(tmpLong, (*it).first, this->GetIdentifier(), this->GetOwnerIdentifier(), (*it).second);
            this->sequence->push_back(tmpEvent);
            tmpLong += (*it).first;
        }

    }


    Trigger::~Trigger()
    {
        cout << "Trigger desctructor" << endl;
        delete(this->sequence);
    }

    string Trigger::GetName()
    {
        return(this->name);
    }

    bool Trigger::GetTransient() 
    {
        return(this->transient);
    }

    void Trigger::SetTransient(bool newValue)
    {
        this->transient = newValue;
    }

    Base::State Trigger::GetState() const
    {
        return this->state;
    }

    void Trigger::SetState(Base::State newState)
    {
        this->state = newState;
        string tmpValue = (newState == Base::ON) ? "ON" : "OFF";

        if (this->action) {
            this->action->Execute(this->GetIdentifier());
        } else {
            //cout << "no action" << endl;;
        }
    }

    Event *Trigger::GetNextEventForSecondsInDay(long SecondsInDay)
    {
        //cout << "     == GetNextEvent for " << this->GetName() << "(" << this->GetIdentifier(true) << ")" <<endl;

        //for(std::vector<Event*>::iterator it = this->sequence->begin(); it != this->sequence->end(); ++it) {
        //        cout << (*(*it)) << endl;
        //}

        for(std::vector<Event*>::iterator it = this->sequence->begin(); it != this->sequence->end(); ++it) {
            if ((*it)->GetSecondsInDay() >= SecondsInDay) {
                (*it)->SetRelativeSecondsToNext((*it)->GetSecondsInDay() - SecondsInDay);
                return (*it);
            }
        }
        this->sequence->at(0)->SetRelativeSecondsToNext(86400 - this->sequence->at(0)->GetSecondsInDay());
        return this->sequence->at(0);
    }

    Event* Trigger::GetLastEventForSecondsInDay(long SecondsInDay)
    {
        //        cout << "     == GetLastEvent for " << this->GetName() << "(" << this->GetIdentifier(true) << ")" <<endl;
        //            for(std::vector<Event*>::reverse_iterator it = this->sequence->rbegin(); it != this->sequence->rend(); ++it) {
        //                cout <<"lala" << endl;
        //                    cout << (*(*it)) << endl;
        //            }

        for(std::vector<Event*>::reverse_iterator it = this->sequence->rbegin(); it != this->sequence->rend(); ++it) {
            if ((*it)->GetSecondsInDay() < SecondsInDay) {
                return (*it);
            }
        }
        return this->sequence->back();
    } 


    string Trigger::GetOwnerIdentifier()
    {
        return this->ownerIdentifier;
    }

    void Trigger::SetOwnerIdentifier(string ownerIdentifier)
    {
        this->ownerIdentifier = ownerIdentifier;

        for(std::vector<Event*>::iterator it = this->sequence->begin(); it != this->sequence->end(); ++it) {
            (*it)->SetTimelineIdentifier(ownerIdentifier);
        } 
    }


    void Trigger::Fire(Event *event, int currentTime)
    {

        if (this->activeStartDate != ULLONG_MAX) {

        }

        this->SetState(event->GetState());
        
        if(this->transient) 
        {
            this->state = Trigger::OFF;
        }
    }

}
