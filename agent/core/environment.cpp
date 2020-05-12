#import <string>

#include <string>
#include <iostream>
#include "environment.h" 
#include "base.h"

using namespace std;


namespace twogreencows_core
{

        string environment::GetPrefix() const
        {
            return "environment";
        }


        environment::environment(string name)
        {
            cout << "Environment creator\n";
            this->SetUpIdentifier();
            this->name = name;
        }
        
        string environment::GetName()
        {
            return(this->name);
        }

        void environment::AddTimeline(timeline tl)
        {
        }
        
        void environment::Start()
        {
        }
}
