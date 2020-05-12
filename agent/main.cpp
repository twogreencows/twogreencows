#include <iostream>

#include <ctime>
#include "Action.h"
#include "Base.hpp"
#include "GrowBox.hpp"
#include "trigger.h"
#include "timeline.h"

using namespace std;
using namespace twogreencows_core;

int main() {
    cout << "Create boxironnement: it starts at a date. It has debug accelerated mode to test!\n";
    cout << " --> Create timeline for light" << endl;
    cout << "       Adding a life cycle 12/12 for a general light starting at 8PMi then switch to 18/6\n";
    cout << "       Adding a life cycle 12/12 for a growing light starting at 8PM\n";
    cout << "       Adding a life cycle 18/6 for a blooming light starting at 8PM\n";
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
    timeline cameraTimeline  = timeline("camera");
    box.AddTimeline(&cameraTimeline);
    cout << "Create timeline:" << cameraTimeline.GetName()+"(" << cameraTimeline.GetIdentifier() << ")\n";

        //light
    Action action1 = Action();
    vector< pair<time_t, trigger::State> > sequence1= {{10800, trigger::ON},{64800, trigger::OFF},{10800, trigger::ON}};
    trigger generalLightTrigger = trigger("led", &action1, sequence1);
    
    Action action2 = Action();
    vector<pair<time_t, trigger::State> > sequence2= {{14600, trigger::ON},{43200, trigger::OFF},{28800, trigger::ON}};   
    trigger growLightTrigger = trigger("grow CFL", &action2, sequence2);
    
    Action action3 = Action();
    vector<pair<time_t, trigger::State> > sequence3= {{10800, trigger::ON},{64800, trigger::OFF},{10800, trigger::ON}};   
    trigger bloomLightTrigger = trigger("bloom CFL", &action3, sequence3);
    
    lightTimeline.ScheduleTrigger(&generalLightTrigger);
    lightTimeline.ScheduleTrigger(&growLightTrigger);   
    lightTimeline.ScheduleTrigger(&bloomLightTrigger);   

        //Camera
    Action action4 = Action();
    vector<pair<time_t, trigger::State> > sequence4= {{79200, trigger::ON},{64800, trigger::OFF}};   
    trigger cameraTrigger = trigger("camera", &action4, sequence4);
    cameraTimeline.ScheduleTrigger(&cameraTrigger);

        //Air
    Action action5 = Action();

    trigger airInTrigger = trigger("airIn", &action5, sequence4);
    
    //base::DumpObjects(); 
    box.Start();
    return(0);
}
