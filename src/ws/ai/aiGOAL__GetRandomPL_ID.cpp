// aiGOAL::GetRandomPL_ID @ 0x832AF040
// ?GetRandomPL_ID@aiGOAL@@UAAFXZ
#include "../../headers/ws/ai/aiGOAL.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/m3d/m3dRND_GEN.h"

// A random valid path-link (waypoint) id from the current nav system, or -1 when there is none.
short aiGOAL::GetRandomPL_ID()
{
    navSYS *ns = navSys;
    if (!ns || ns->GetNWP() <= 0)
        return -1;
    short nwp = (short)navSys->GetNWP();
    return (short)m3dRndGen.RndIntMax(nwp - 1);
}
