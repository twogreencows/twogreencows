#include <iostream>

#include "base.h"
#include "GrowBox.hpp"
#include "timeline.h"

using namespace std;
using namespace twogreencows_core;

int main() {
    cout << "Create boxironnement: it starts at a date. It has debug accelerated mode to test!\n";
    cout << " --> Create timeline for light" << endl;
    cout << "       Adding a life cycle 14/8 for a general light starting at 8PMi then switch to 12/12\n";
    cout << "       Adding a life cycle 12/12 for a blooming light starting at 8PM\n";
    cout << "       Adding a life cycle 14/8 for a growing light starting at 8PM\n";
    cout << "       The change of light is at start date plus 8 weeks. It is not automatic but triggers a warning \n";
    cout << " --> Create a timeline for air!\n";
    cout << "       It has a extractor and an in flux. They follow light cycle but when dark they make every 15 minutes\n";

    cout << "Creatinge GrowBox\n";
    GrowBox box = GrowBox("greenhouse 1");
    cout << "Create GrowBox:" << box.GetName()+"(" << box.GetIdentifier() << ")\n";
    timeline lightTimeline = timeline("light");
    box.AddTimeline(&lightTimeline);
    cout << "Create timeline:" << lightTimeline.GetName()+"(" << lightTimeline.GetIdentifier() << ")\n";
    timeline airTimeline  = timeline("air");
    box.AddTimeline(&airTimeline);
    cout << "Create timeline:" << airTimeline.GetName()+"(" << airTimeline.GetIdentifier() << ")\n";
    trigger generalLightTrigger = trigger("led");
    trigger growLightTrigger = trigger("grow CFL");
    lightTimeline.ScheduleTrigger(&generalLightTrigger);
    lightTimeline.ScheduleTrigger(&growLightTrigger);   
 
    trigger bloomLightTrigger = trigger("bloom CFL");

    lightTimeline.ScheduleTrigger(&bloomLightTrigger);   
    
    //base::DumpObjects(); 
    box.Start();
    return(0);
}
