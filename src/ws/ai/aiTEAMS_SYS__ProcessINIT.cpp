#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/gs/gsTEAMS.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiTEAMS_SYS::ProcessINIT() @ 0x83173AC8
// ?ProcessINIT@aiTEAMS_SYS@@QAAXXZ
// Install the default teams property-script ("default_teams.ps") into the gs team registry.
void aiTEAMS_SYS::ProcessINIT()
{
    dsTSTRING<char> defaultPS("default_teams.ps");
    gsTEAMS::Instance().SetDefaultPS(defaultPS);
}
