
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
            this->name = infoName.nodename;
            //cout << infoName.machine <<endl;
            //cout << infoName.nodename <<endl;
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
