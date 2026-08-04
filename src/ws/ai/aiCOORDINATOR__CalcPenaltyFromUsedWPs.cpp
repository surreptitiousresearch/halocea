#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPlanner
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/navWFUNC_SQAS_boundary.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dDist_2

// aiCOORDINATOR::CalcPenaltyFromUsedWPs @ 0x83235E18
// ?CalcPenaltyFromUsedWPs@aiCOORDINATOR@@IBAMFABUm3dV@@ABVnavWFUNC_SQAS@@PAVaiBRAIN@@@Z
//
// Penalty for standing at (wpid, wpPos) given the waypoints other brains have already claimed:
// +30 for an exact wpid collision with another brain, otherwise a distance-weighted term from
// each other brain's claimed WP. The claiming brain itself (pBrainSelf) is skipped.
float aiCOORDINATOR::CalcPenaltyFromUsedWPs(short wpid, const m3dV &wpPos,
                                            const navWFUNC_SQAS &wfun, aiBRAIN *pBrainSelf) const
{
    typedef ds::impl::MAP_ITERATOR<aiBRAIN *, short, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
        ResultIter;

    float penalty = 0.0f;
    // The reversed method is const but walks the (non-const) map; mirror that access here.
    ds::MAP<aiBRAIN *, short, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> &used =
        const_cast<ds::MAP<aiBRAIN *, short, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> &>(resultsUsed);

    for (ResultIter it(used.Begin()); !it.IsDone(); ++it)
    {
        if (it.key() == pBrainSelf)
            continue;

        short usedWpid = *it.value();
        if (wpid == usedWpid)
        {
            penalty += 30.0f;
        }
        else
        {
            navWP *wp = aiPlanner->GetWP((unsigned short)usedWpid);
            if (wp)
            {
                m3dV effPos;
                wp->GetEffectivePos(&effPos);
                float distSq = m3dDist_2(&wpPos, &effPos);
                penalty += wfun.GetWeight_D2(distSq);
            }
        }
    }
    return penalty;
}
