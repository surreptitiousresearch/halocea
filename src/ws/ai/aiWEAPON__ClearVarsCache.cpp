#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h" // m3dVZero (via m3d_boundary.h)
#include "../../headers/ws/ai/aiS_VARS.h"

// aiWEAPON::ClearVarsCache @ 0x83294570
// ?ClearVarsCache@aiWEAPON@@UAAXXZ
//
// Invalidate every strike-vars cache entry: drop the cached weapon-interface and controller weak
// references, clear the muzzle tip pos/dir, and mark the cache invalid.
void aiWEAPON::ClearVarsCache()
{
    for (int i = 0; i < svarsList.nEntry; ++i) {
        aiS_VARS &v = svarsList[i];
        v.iface.Release();
        v.ctrl.Release();
        v.posTip = m3dVZero;
        v.dirTip = m3dVZero;
        v.isCacheValid = false;
    }
}
