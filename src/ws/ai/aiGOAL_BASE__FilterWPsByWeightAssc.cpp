#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner
#include "../../headers/ws/ai/aiDEBUG_boundaries.h" // navASSOC_INFO
#include "../../headers/ws/nav/navWFUNC_DIST.h"
#include "../../headers/ws/nav/navWFUNC.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/navWP_ID_WEIGHT.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsCMP.h"
#include "../../headers/ws/ds/dsSWAP.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dVZero
#include "../../headers/apCL.h"

// aiGOAL_BASE::FilterWPsByWeightAssc(...) @ 0x8329C190
// ?FilterWPsByWeightAssc@aiGOAL_BASE@@SAXPAVanimINST@@PBVnavWFUNC_DIST@@MH_NABUm3dV@@KABV?$dsVECTOR@
//   UnavWP_ID_WEIGHT@@$07@@AAV5@PAV?$dsVECTOR@UnavASSOC_INFO@@$07@@@Z
//
// Reweight + ascending-sort the candidate waypoints like FilterWPsByWeight, then walk up to
// 3*maxCount of the best candidates and keep up to `maxCount` that (a) pass the HO-acquirability
// gate and (b) are thin-visible from `wfun`'s center (temporarily shifted by `shift`). When
// `dbgAssoc` is supplied, one association record per examined candidate is appended to it.
void aiGOAL_BASE::FilterWPsByWeightAssc(animINST *pInst, const navWFUNC_DIST *wfun, float maxWeight,
                                        int maxCount, bool gateHO, const m3dV &shift,
                                        unsigned int mask,
                                        const dsVECTOR<navWP_ID_WEIGHT, 8> &src,
                                        dsVECTOR<navWP_ID_WEIGHT, 8> &dst,
                                        dsVECTOR<navASSOC_INFO, 8> *dbgAssoc)
{
    const int cap = dst.allocated;
    dst.nElem = 0;
    if (src.nElem > cap)
        dst.Realloc(src.nElem);

    const int srcCount = src.nElem;

    apCL cl = { "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_gobase.cpp", 143 };
    dsVECTOR<navWP_ID_WEIGHT, 8> scored(cl);

    navWFUNC *wf = const_cast<navWFUNC *>(static_cast<const navWFUNC *>(wfun));
    for (int i = 0; i < srcCount; ++i)
    {
        const unsigned short wpid = static_cast<unsigned short>(src[i].wpid);
        const float baseWeight = src[i].weight;

        navWP *wp = aiPlanner->GetWP(wpid);
        if (!wp || !wp->IsEnabled(mask))
            continue;

        m3dV pos;
        wp->GetEffectivePos(&pos);
        float w = wf->__vftable->GetWeight(wf, wp, &pos, static_cast<short>(wpid));
        if (w < maxWeight)
        {
            navWP_ID_WEIGHT e;
            e.weight = w + baseWeight;
            e.wpid = static_cast<short>(wpid);
            scored.PushBack(e);
        }
    }

    {
        dsCMP cmp;
        dsSWAP<navWP_ID_WEIGHT> swap;
        scored.Sort(cmp, swap);
    }

    const int total = scored.nElem;
    m3dV center = wfun->center;

    if (maxCount < 0)
        maxCount = total;
    int candidateCap = total;
    if (total >= 3 * maxCount)
        candidateCap = 3 * maxCount;

    int accepted = 0;
    for (int idx = 0; idx < candidateCap; ++idx)
    {
        if (accepted >= maxCount)
            break;

        navWP *wp = aiPlanner->GetWP(static_cast<unsigned short>(scored[idx].wpid));

        // Skip HO waypoints this acquirer cannot take (only when the HO gate is enabled).
        if (gateHO && wp->IsHO() && !wp->CanAcquireHO(0xFFFFFFFFu, mask))
            continue;

        // Apply the temporary visibility-check shift and the "thin"/pass flag (0x200) for the test.
        wp->visCheckAdditionalShift.x = shift.x;
        wp->visCheckAdditionalShift.y = shift.y;
        wp->visCheckAdditionalShift.z = shift.z;
        unsigned int st = wp->flags.state;
        unsigned int newState = st | 0x200u;
        if (((~mask >> 1) & 1) == 0)
            newState = st & ~0x200u;
        wp->flags.state = newState;

        int visible = wp->IsVisibleThin(&center, nullptr, pInst);

        if (dbgAssoc)
        {
            m3dV to;
            wp->GetPosVisChk(&center, &to, 0.0f);
            navASSOC_INFO info;
            info.isVisible = (visible != 0);
            info.vFrom = center;
            info.vTo = to;
            dbgAssoc->PushBack(info);
        }

        wp->flags.state &= ~0x200u;
        wp->visCheckAdditionalShift = m3dVZero;

        if (visible)
        {
            dst.PushBack(scored[idx]);
            ++accepted;
        }
    }
}
