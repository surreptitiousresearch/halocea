#include "../../headers/ws/ai/aiAIM_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // m3dUpdateTimeField
#include "../../headers/ws/m3d/m3dV.h"

// aiAIM_COMMON::ProcessFrame @ 0x83299F18
// ?ProcessFrame@aiAIM_COMMON@@UAAXXZ
//
// Per-frame aim update: age the freeze timer, recompute the basic aim position, then apply the aim
// modifiers to both the aim and weapon-target copies and publish them.
void aiAIM_COMMON::ProcessFrame()
{
    float elapsed = pBrain->GetElapsedTime();
    m3dUpdateTimeField(&pBrain->st.pointee->aim.timerToFreezeAim, elapsed);

    UpdateBasicPos();

    posForProjectile = posBasic;

    m3dV posAim = posBasic;
    m3dV posWpnTgt = posBasic;
    ApplyModifiers(posAim, posWpnTgt);
    FillResults(posAim, posWpnTgt);
}
