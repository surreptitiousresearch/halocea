#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner
#include "../../headers/ws/nav/navWFUNC.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/navWP_ID_WEIGHT.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsCMP.h"
#include "../../headers/ws/ds/dsSWAP.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/apCL.h"

// aiGOAL_BASE::FilterWPsByWeight(...) @ 0x8329BD28
// ?FilterWPsByWeight@aiGOAL_BASE@@SAXPBVnavWFUNC@@MABV?$dsVECTOR@UnavWP_ID_WEIGHT@@$07@@AAV3@_NH@Z
//
// For each waypoint in `src`, recompute its weight with `wfun` (added to the incoming weight); keep
// those under `maxWeight`; optionally sort ascending; then copy up to `maxCount` (all when <0) into
// `dst`. Disabled/missing waypoints are dropped.
void aiGOAL_BASE::FilterWPsByWeight(const navWFUNC *wfun, float maxWeight,
                                    const dsVECTOR<navWP_ID_WEIGHT, 8> &src,
                                    dsVECTOR<navWP_ID_WEIGHT, 8> &dst,
                                    bool doSort, int maxCount)
{
    const int cap = dst.allocated;
    dst.nElem = 0;
    if (src.nElem > cap)
        dst.Realloc(src.nElem);

    const int srcCount = src.nElem;

    apCL cl = { "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_gobase.cpp", 91 };
    dsVECTOR<navWP_ID_WEIGHT, 8> scored(cl);

    for (int i = 0; i < srcCount; ++i)
    {
        const unsigned short wpid = static_cast<unsigned short>(src[i].wpid);
        const float baseWeight = src[i].weight;

        navWP *wp = aiPlanner->GetWP(wpid);
        if (!wp || !wp->IsEnabled(6u))
            continue;

        m3dV pos;
        wp->GetEffectivePos(&pos);
        float w = const_cast<navWFUNC *>(wfun)->__vftable->GetWeight(
            const_cast<navWFUNC *>(wfun), wp, &pos, static_cast<short>(wpid));
        if (w < maxWeight)
        {
            navWP_ID_WEIGHT e;
            e.weight = w + baseWeight;
            e.wpid = static_cast<short>(wpid);
            scored.PushBack(e);
        }
    }

    if (doSort)
    {
        dsCMP cmp;
        dsSWAP<navWP_ID_WEIGHT> swap;
        scored.Sort(cmp, swap);
    }

    int count = scored.nElem;
    if (maxCount < 0 || maxCount >= count)
        maxCount = count;
    for (int i = 0; i < maxCount; ++i)
        dst.PushBack(scored[i]);
}
