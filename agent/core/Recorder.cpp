#import <string>
#import <Recorder.hpp>

using namespace std;

namespace twogreencows_core
{
     Recorder::Recorder()
     {
        
     }
      
     string Recorder::GetPrefix() const
      {
          return "recorder";
      }

}
