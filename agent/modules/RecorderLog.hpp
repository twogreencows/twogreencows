#pragma once

#include <fstream>

using namespace std;

#include "Base.hpp"
#include "DataPoint.hpp"
#include "Recorder.hpp"


namespace twogreencows_core
{
    class RecorderLog:public Recorder {
    private:
        string currentFile;
        ofstream LogFile;

    public:
        virtual string GetSubtype() const;
        RecorderLog(string GrowboxIdentifier);
        ~RecorderLog();

        virtual void Open();
        virtual void LogDataPoint(DataPoint dp);
        virtual void Close();
    };

}
