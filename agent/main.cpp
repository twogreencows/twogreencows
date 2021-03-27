#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <unordered_map>


#include <cstdlib>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>


#include <ctime>
#include "Action.hpp"
#include "ActionRelay.hpp"
#include "ActionCamera.hpp"
#include "ActionTemperatureSensor.hpp"
#include "Base.hpp"
#include "Growbox.hpp"
#include "GrowboxManager.hpp"
#include "Trigger.hpp"
#include "Timeline.hpp"


using namespace std;
using namespace twogreencows_core;
using namespace rapidjson;

int main(int argc, char *argv[]) {
    string TGCFilePath = "";
    string TGCFullFilePath = "";
    string LastActiveGrowboxIdentifier="";
    Growbox *box;
    unordered_map<string,string > dependancies;

    //In a first version, only one growbox can be run by launch. 
    //If no argument is given the last one is used, if there is one
    //if there is a --tgcfile argument a new growbox is created. 
    //In such case older ones are not discarded/removed as a future version may handle multi all.

    if (argc == 1) {
        cerr << "== No path to TGC file descriptor found. Will use last one stored or bail out if not" << endl;
        LastActiveGrowboxIdentifier = GrowboxManager::GetSharedManager()->GetLastActiveGrowboxIdentifier();
        cout << "> Last identifier is " << LastActiveGrowboxIdentifier << endl;
        if (LastActiveGrowboxIdentifier.empty()) {
            cerr << "No descriptor file found" << endl;
            return 0;
        }
        box = GrowboxManager::GetSharedManager()->CreateGrowboxFromPersistentStorage(LastActiveGrowboxIdentifier);
    } else {
        for (int i = 1; i < argc; ++i) {
            if (std::string(argv[i]) == "--help") {
                cerr <<"The twogreencows agent can run only one growbbox at a time." << endl;
                cerr << "- If no argument is given the agent is looking if there was a previously running growbox and launches it." << endl;
                cerr << "- If there is a --tgcfile argument,a new growbox is created and launched." << endl;
                cerr << "In this last case previously created growboxes are not discarded/deleted as a future version" <<endl; 
                cerr << "of the agent may handle more than one at a time." << endl;
                return 0;
            } else if (std::string(argv[i]) == "--tgcfile") {
                if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                    TGCFilePath = string(argv[++i]);
                }
            }  
        }
        if (TGCFilePath.empty()) {
            cerr << "No descriptor file found" << endl;
            return 0;
        }

        if (TGCFilePath[0] == '/') {
            TGCFullFilePath = TGCFilePath;
        } else {
            TGCFullFilePath = filesystem::current_path().string() + "/" + TGCFilePath;
        }
        box = GrowboxManager::GetSharedManager()->CreateGrowboxFromDescriptorFile(TGCFullFilePath);
    }
    if (nullptr != box) {
        box->Start();
    }
    return(0);
}
