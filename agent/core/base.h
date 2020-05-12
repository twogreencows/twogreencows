#pragma once


using namespace std;

#include <unordered_map>

namespace twogreencows_core
{
    class base{
            string identifier;
            static unordered_map<string, base> umap;        
        public:

        virtual string  getPrefix() const = 0 ; 
        string getIdentifier() const;
        void SetUpIdentifier();
        base();
    };
}
