	

#include <fstream>
#include <iostream>

using namespace std;

#include "Base.hpp"
#include "DataPoint.hpp"
#include "Recorder.hpp"
#include "RecorderNetwork.hpp"

namespace twogreencows_core
{

        string RecorderNetwork::GetSubtype() const
        {
            return "network";
        }

        RecorderNetwork::RecorderNetwork(string GrowboxIdentifier):Recorder(GrowboxIdentifier)
        {
        }


       void RecorderNetwork::Open()
        {
        }

        void RecorderNetwork::LogDataPoint(DataPoint dp)
        {
        }

        void RecorderNetwork::Close()
        {
        }

}
