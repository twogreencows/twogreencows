
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/utsname.h>

using namespace std;

#include "Base.hpp"
#include "Device.hpp"

namespace twogreencows_core
{
    Device::Device(string name)
    {
        this->SetUpIdentifier();
        this->ObjectVersion = 1; 
        if (false == name.empty()) {
            this->name = name;
        } else {
            struct utsname infoName;
            uname(&infoName);
            std::string localString(".local");
            this->name = infoName.nodename;
            if (this->name.length() > localString.length()) {
                if (0 == this->name.compare(this->name.length() - localString.length(), localString.length(), localString)) {
                    this->name = this->name.substr(0, this->name.size()-localString.size());
                }
            }  
       }
    }

    string Device::GetClassPrefix() const
    {
        return "device";
    }
    
    int Device::GetClassVersion() const
    {
        return 1;
    }


    string Device::GetName()
    {
        return this->name;
    }
}
