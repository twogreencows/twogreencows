#import <string>

#include <string>
#include <iostream>
#include "trigger.h"
#include "Action.h"

using namespace std;


namespace twogreencows_core
{

        string trigger::GetPrefix() const
        {
            return "trigger";
        }


        trigger::trigger(string name, Action *action)
        {
            this->SetUpIdentifier();
            this->name = name;
        }
        
        string trigger::GetName()
        {
            return(this->name);
        }

        
}
