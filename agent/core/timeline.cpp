#import <string>

#include <string>
#include <iostream>
#include "timeline.h" 
#include "base.h"

using namespace std;


namespace twogreencows_core
{

        string timeline::GetPrefix() const
        {
            return "timeline";
        }


       timeline::timeline(string name)
        {
            cout << "timeline creator\n";

            this->SetUpIdentifier();
            this->name = name;
        }
       
       string timeline::GetName()
        {
            return this->name;
        }
}

