#pragma once

#include <string>
#include "Base.hpp"
#include "DataPoint.hpp"


using namespace std;

namespace twogreencows_core
{
    class Recorder:public Base {
    private:
        string GrowboxIdentifier;
    protected:
        bool IsOpened;

    public:
        enum Kind{
            LOG,
            NETWORK
        };

        static Recorder *CreateRecorder(Recorder::Kind kind, string GrowBoxIdentifier);

        virtual string GetClassPrefix() const;
        virtual string GetSubtype() const = 0;
        Recorder(string GrowBoxIdentifier);
        
        virtual int GetClassVersion() const;
        string GetGrowBoxIdentifier();
        virtual void Open() = 0;
        virtual void LogDataPoint(DataPoint dp) = 0;
        virtual void Close() = 0;
    };

}
