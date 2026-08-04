#include "../../headers/ws/ai/aiSND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/m3d/m3dSPL.h" // nKp

// aiSND::ProcessInit @ 0x832997A8
// ?ProcessInit@aiSND@@UAAXXZ
//
// Per-spawn init: resolve the voice type from the owning brain's descriptor, then enable the pitch
// response only when the descriptor's pitch curve carries more than one keyframe.
void aiSND::ProcessInit()
{
    aiBRAIN_DESC *desc = reinterpret_cast<aiBRAIN_DESC *>(pBrain->spDesc.pointee);

    SetVoiceType(desc->GetVoiceType());

    int val = permissions.val;
    int newVal = val | AI_SNDP_ENABLE_PITCH;

    // Big-endian idiom: (nKp>=0)+(nKp<=1) sums to 2 (even -> low bit clear) exactly when the curve
    // has <=1 keyframes, in which case the pitch response is disabled.
    int nKp = reinterpret_cast<const m3dSPL *>(&desc->curveSndPitch)->nKp;
    if ((((nKp >= 0) + (nKp <= 1)) & 1) == 0)
        newVal = val & ~AI_SNDP_ENABLE_PITCH;

    permissions.val = newVal;
}
