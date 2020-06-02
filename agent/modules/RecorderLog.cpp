

#include <fstream>
#include <iostream>

using namespace std;

#include <errno.h>

#include "Base.hpp"
#include "DataPoint.hpp"
#include "Growbox.hpp"
#include "RecorderLog.hpp"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>

using namespace rapidjson;

namespace twogreencows_core
{

        string RecorderLog::GetSubtype() const
        {
            return "file";
        }

        RecorderLog::RecorderLog(string GrowboxIdentifier):Recorder(GrowboxIdentifier)
        {
            
        }

        RecorderLog::~RecorderLog() 
        {
        }

       void RecorderLog::Open()
        {
            Growbox *owningGrowBox = static_cast<twogreencows_core::Growbox*>(Base::ObjectWithIdentifier(this->GetGrowBoxIdentifier()));
            struct stat st = {0};
            string logPath = owningGrowBox->GetDataFolderPath()+"Logs/";
            if (stat(logPath.c_str(), &st) == -1) {
                mkdir(logPath.c_str(), 0744);
            }
            this->IsOpened = true;
        }

        void RecorderLog::LogDataPoint(DataPoint dp)            
        {
            if (false == this->IsOpened)
                return;
            
            Growbox *owningGrowBox = static_cast<twogreencows_core::Growbox*>(Base::ObjectWithIdentifier(this->GetGrowBoxIdentifier()));
            string logFolderPath = owningGrowBox->GetDataFolderPath()+"Logs/";

            string daySuffix = dp.GetDeviceDatePrefix();
            if (this->currentFile.empty()) {
                this->currentFile = daySuffix;


                this->LogFile.open (logFolderPath+daySuffix+".log", ios::out | ios::app);
                //this->LogFile.open (daySuffix+".log", ios::out | ios::app);

                if (!this->LogFile) {
                    cerr << "Error opening" << endl;
                    cerr << errno << endl;
                }

            } else if (0 != daySuffix.compare(currentFile)) {
                this->currentFile = daySuffix;
                this->LogFile.close();
                this->LogFile.open (logFolderPath+daySuffix+".log", ios::out | ios::app);
            } 
            

            StringBuffer sb;
            Writer<StringBuffer> writer(sb);
            writer.StartObject();
            dp.Serialize(writer);
            writer.EndObject();
            this->LogFile << sb.GetString() << endl;
        }

        void RecorderLog::Close()
        {
            this->IsOpened = false;
        }

}
