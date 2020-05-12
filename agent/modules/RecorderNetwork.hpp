#pragma once

#include <fstream>

using namespace std;

#include "Base.hpp"
#include "DataPoint.hpp"
#include "Recorder.hpp"


namespace twogreencows_core
{
    class RecorderNetwork:public Recorder {
        private:

    public:
        virtual string GetSubtype() const;
        RecorderNetwork(string GrowboxIdentifier);

        virtual void Open();
        virtual void LogDataPoint(DataPoint dp);
        virtual void Close();
    };

}
