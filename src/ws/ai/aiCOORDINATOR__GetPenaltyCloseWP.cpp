#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/nav/navWFUNC_SQAS_boundary.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiCOORDINATOR::GetPenaltyCloseWP @ 0x83236990
// ?GetPenaltyCloseWP@aiCOORDINATOR@@UBAMFABUm3dV@@ABVnavWFUNC_SQAS@@PAVaiBRAIN@@@Z
//
// Penalty for occupying a WP close to WPs already in use: the used-WP term for this brain, plus a
// per-group side-effect pass (each group recomputes its own close-WP state). Negative wpid => 0.
float aiCOORDINATOR::GetPenaltyCloseWP(short wpid, const m3dV &wpPos,
                                       const navWFUNC_SQAS &wfun, aiBRAIN *pBrainSelf) const
{
    if (wpid < 0)
        return 0.0f;

    float penalty = CalcPenaltyFromUsedWPs(wpid, wpPos, wfun, pBrainSelf);

    int nElem = groups.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiCOORD_GROUP *grp = groups[i];
        // vtable+92: per-group close-WP update — slot not individually resolved this batch, so
        // dispatch through the raw vtable offset the caller used (result is discarded).
        (*reinterpret_cast<void (**)(aiCOORD_GROUP *)>(
            *reinterpret_cast<char **>(grp) + 92))(grp);
    }
    return penalty;
}
