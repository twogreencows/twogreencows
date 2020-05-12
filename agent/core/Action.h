#pragma once

#include "Base.hpp"

using namespace std;


namespace twogreencows_core
{
    class Action:Base{
            string identifier;
    
    public:
        
        Action();
        void Execute(string triggerUUID);
        virtual string GetPrefix() const;
    };
}
