#include <iostream>

#include "base.h"
#include "environment.h"
#include "timeline.h"

using namespace std;
using namespace twogreencows_core;

int main() {
    cout << "Create environnement: it starts at a date. It has debug accelerated mode to test!\n";
    cout << " --> Create timeline for light\n";
    cout << "       Adding a life cycle 14/8 for a general light starting at 8PMi then switch to 12/12\n";
    cout << "       Adding a life cycle 12/12 for a blooming light starting at 8PM\n";
    cout << "       Adding a life cycle 14/8 for a growing light starting at 8PM\n";
    cout << "       The change of light is at start date plus 8 weeks. It is not automatic but triggers a warning \n";
    cout << " --> Create a timeline for air!\n";
    cout << "       It has a extractor and an in flux. They follow light cycle but when dark they make every 15 minutes\n";

    cout << "Create environment\n";
    environment env = environment("greenhouse 1");
    cout << "Create environment:" << env.GetName()+"(" << env.GetIdentifier() << ")\n";
    timeline lightTimeline = timeline("light");
    //environment.AddTimeline(lightTimeline)
    cout << "Create timeline:" << lightTimeline.GetName()+"(" << lightTimeline.GetIdentifier() << ")\n";
    timeline airTimeline  = timeline("air");
    //environment.AddTimeline(airTimeline)
    environment.Start();
    cout << "Create timeline:" << airTimeline.GetName()+"(" << airTimeline.GetIdentifier() << ")\n";
    return(0);
}
