#include <iostream>

#include <ctime>
#include "Action.h"
#include "Base.hpp"
#include "GrowBox.hpp"
#include "Trigger.hpp"
#include "Timeline.hpp"

using namespace std;
using namespace twogreencows_core;

int main() {
    cout << "Create growbox: it starts at a date. It has debug accelerated mode to test!\n";
    cout << " --> Create timeline for light" << endl;
    cout << "       Adding a life cycle 18 ON/6 OFF for a general light starting at 6PM then switch to 12/12\n";
    cout << "       Adding a life cycle 12 ON/12 OFF for a growing light starting at 6PM\n";
    cout << "       Adding a life cycle 18 ON/6 OFFfor a blooming  light starting at 8PM\n";
    cout << "       The change of light is at start date plus 8 weeks. It is not automatic but triggers a warning \n";
    cout << " --> Create a timeline for air!\n";
    cout << "       It has a extractor and an in flux. They follow light cycle but when dark they make every 15 minutes\n";
    cout << " --> Create a timeline for camera!\n";
    cout << "       It takes a picture a day at 10 PM\n";

    cout << "Creating GrowBox\n";
    GrowBox box = GrowBox("greenhouse 1");
    cout << "Create GrowBox:" << box.GetName() + " " << box << endl ;
    Timeline lightTimeline = Timeline("light");
    box.AddTimeline(&lightTimeline);
    cout << "Create timeline:" << lightTimeline.GetName()+"(" << lightTimeline.GetIdentifier() << ")\n";
    Timeline airTimeline  = Timeline("air");
    box.AddTimeline(&airTimeline);
    cout << "Create timeline:" << airTimeline.GetName()+"(" << airTimeline.GetIdentifier() << ")\n";
    Timeline cameraTimeline  = Timeline("camera");
    box.AddTimeline(&cameraTimeline);
    cout << "Create timeline:" << cameraTimeline.GetName()+"(" << cameraTimeline.GetIdentifier() << ")\n";

        //light
        //8PM is 8*3600 7200, 8AM is 8*3600
    Action action1 = Action();
    vector< pair<time_t, Base::State> > sequence1= {{4*3600, Base::ON},{12*3600, Base::OFF},{8*3600, Base::ON}};
    Base::DumpSequence(sequence1);
    Trigger generalLightTrigger = Trigger("led", &action1, sequence1);
    
    Action action2 = Action();
    vector<pair<time_t, Base::State> > sequence2= {{4*3600, Base::ON},{12*3600, Base::OFF},{8*3600, Base::ON}};   
    //Base::DumpSequence(sequence2);
    Trigger growLightTrigger = Trigger("grow CFL", &action2, sequence2);
    
    Action action3 = Action();
    vector<pair<time_t, Base::State> > sequence3= {{5*3600, Base::ON},{14*3600, Base::OFF},{5*3600, Base::ON}};   
    Base::DumpSequence(sequence3);
    Trigger bloomLightTrigger = Trigger("bloom CFL", &action3, sequence3);
    
    lightTimeline.ScheduleTrigger(&generalLightTrigger);
    lightTimeline.ScheduleTrigger(&growLightTrigger);   
    lightTimeline.ScheduleTrigger(&bloomLightTrigger);   

        //Camera
    Action action4 = Action();
    vector<pair<time_t, Base::State> > sequence4= {{22*3600, Base::OFF}, {2*3600, Base::ON}};   
    Base::DumpSequence(sequence4, true);
    Trigger cameraTrigger = Trigger("camera", &action4, sequence4, true);
    cameraTimeline.ScheduleTrigger(&cameraTrigger);

        //Air
    Action action5 = Action();

    vector<pair<time_t, Trigger::State> > sequence5= {{14400, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {2700, Base::OFF}, {900, Base::ON},
        {14400, Base::ON}};   
    Base::DumpSequence(sequence5, false);
    Trigger airInTrigger = Trigger("air-in", &action5, sequence5);
    Trigger airOutTrigger = Trigger("air-out", &action5, sequence5);
    airTimeline.ScheduleTrigger(&airInTrigger); 
    airTimeline.ScheduleTrigger(&airOutTrigger); 
    box.Start();
    return(0);
}
