#include <string>
#include "Base.hpp"
#include "Hardware.hpp"

using namespace std;

namespace twogreencows_core
{
     Hardware::Hardware(string name)
     {
         this->name = name;
     }
      
     string Hardware::GetPrefix() const
      {
          return "hardware";
      }

      string Hardware::GetName()
      {
          return this->name;
      }
}
