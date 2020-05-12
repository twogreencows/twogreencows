#include <string>
#include <iostream>
#include "Base.hpp"
#include "Device.hpp"
#include <sys/types.h>
#include <sys/utsname.h>

using namespace std;

namespace twogreencows_core
{
    Device::Device(string name)
    {
        this->SetUpIdentifier();
        if (false == name.empty()) {
            this->name = name;
        } else {
            struct utsname infoName;
            uname(&infoName);
            this->name = infoName.nodename;
            cout << infoName.machine <<endl;
            cout << infoName.nodename <<endl;
       }
    }

    string Device::GetPrefix() const
    {
        return "device";
    }

    string Device::GetName()
    {
        return this->name;
    }
}
