#include <string>

#include "Recorder.hpp"
#include "RecorderLog.hpp"
#include "RecorderNetwork.hpp"

using namespace std;

namespace twogreencows_core
{
     Recorder::Recorder(string GrowboxIdentifier)
     {
        this->GrowboxIdentifier = GrowboxIdentifier;
     }
      
     string Recorder::GetClassPrefix() const
      {
          return "recorder";
      }

    int Recorder::GetClassVersion() const
    {
        return 1;
    }

    Recorder *Recorder::CreateRecorder(Recorder::Kind kind, string GrowBoxIdentifier)
    {
        Recorder *result = NULL;
        if ( kind == Recorder::LOG) {
            result = new RecorderLog(GrowBoxIdentifier);
        } else if (kind == Recorder::NETWORK) {
            result = new RecorderNetwork(GrowBoxIdentifier);
        }
        return result;

    }
    
    string Recorder::GetGrowBoxIdentifier()
    {
        return this->GrowboxIdentifier;
    }

    void Recorder::LogDataPoint(DataPoint dp )
    {
        
    }

}
