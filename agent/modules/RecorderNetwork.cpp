	

#include <fstream>
#include <iostream>

using namespace std;

#include "Base.hpp"
#include "DataPoint.hpp"
#include "Recorder.hpp"
#include "RecorderNetwork.hpp"


#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include "mdns.h"

#if 0
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#endif

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
            //for each record send to the server
        }

        void RecorderNetwork::Close()
        {
        }

}
