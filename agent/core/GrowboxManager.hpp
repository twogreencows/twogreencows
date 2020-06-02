//
//  Project: Twogreencows Agent
//  File: GrowboxManager.hpp
//
//  Description: Declaration file for GrowboxManager. This component is responsible
//  for the management of growboxes. It also handles communication with persistent
//  storage.In version 1 only one growbox can beb active at a time.
//
//  Copyright TwoGreenCows 2020
//




#pragma once

#include <string>
#include "Growbox.hpp"
#include <sqlite3.h>

namespace twogreencows_core
{
    class GrowboxManager
    {
        private:
            static GrowboxManager *SharedManager;
            GrowboxManager();
            std::string TwoGreenCowsMainFolderPath;
            
            sqlite3 *OpenedDB;
            std::string OpenedIdentifier;

        public:
            static GrowboxManager *GetSharedManager();
            std::string GetLastActiveGrowboxIdentifier();
            std::string GetTopFolderPathForGrowboxIdentifier(std::string GrowboxIdentifier);
            void SetLastActiveGrowboxIdentifier(std::string GrowboxIdentifier);
            
            Growbox *CreateGrowboxFromDescriptorFile(std::string PathToDescriptorFile, bool PersistToStorage=true);        
            Growbox *CreateGrowboxFromPersistentStorage(std::string GrowboxIdentifier);   

            void BuildDataBaseForIdentifier(std::string GrowbboxIdentifier); 
            void OpenDataBaseForIdentifier(std::string GrowboxIdentifier);
            void CloseDataBaseForIdentifier(std::string GrowboxIdentifier);
            sqlite3 *GetOpenedDataBase();            
    };
}
