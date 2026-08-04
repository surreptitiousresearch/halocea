#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_boundary.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/nav/navWP.h"

// aiPLANNER::SelectNavSysIdx(dsVECTOR<navSYS*,8>&, const m3dV&, const char*) const — 0x83167008
// When a unit's position is claimed by several nav-systems, pick the one whose closest enabled
// waypoint is nearest to `pos`. `bname` is the owning unit's name (used only for logging). The
// PPC decompiler split the (m3dV& pos, const char* bname) pair into __int64 halves and invented
// the variadic _apLog scratch slots; both are reconstructed to their true ABI here.
int aiPLANNER::SelectNavSysIdx(dsVECTOR<navSYS *, 8> &acceptedNavSysList,
                               const m3dV &pos, const char *bname) const
{
    if (acceptedNavSysList.nElem <= 1)
        return 0;

    _apLog("~AI,Error~%s: Attention! Conflict nav-systems. NS with closest WP will be accepted.",
           bname);

    // Accumulate a human-readable ", "-joined list of the candidate nav-system names for logging.
    dsTSTRING<char> nsListStr;
    nsListStr.pBuffer = nullptr;
    nsListStr.UnsafeInitEmpty();

    int   bestIdx = 0;
    float bestDist = 2000000.0f;

    int nElem = acceptedNavSysList.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        navSYS *ns = acceptedNavSysList[i];

        float closestWpDist = 2000000.0f;
        int   nwp = ns->GetNWP();
        for (short w = 0; w < nwp; ++w)
        {
            navWP *wp = ns->GetWP(w);
            if (!wp)
                continue;
            if (wp->IsEnabled(6u) && (wp->flags.state & 0x80000000) == 0)
            {
                float d = wp->Dist_2(&pos);
                if (d < closestWpDist)
                    closestWpDist = d;
            }
        }

        if (closestWpDist < bestDist)
        {
            bestDist = closestWpDist;
            bestIdx = i;
        }

        if (i > 0)
            nsListStr.Insert(nsListStr.pBuffer->strLen, ", ", -1);
        nsListStr.Insert(nsListStr.pBuffer->strLen, ns->nameSys.pBuffer->str, -1);
    }

    _apLog("~AI~%s: NS list: %s", bname, nsListStr.pBuffer->str);
    _apLog("~AI~%s: selected: %s", bname, acceptedNavSysList[bestIdx]->nameSys.pBuffer->str);

    if (--nsListStr.pBuffer->refCount == 0)
        dlFree(nsListStr.pBuffer);

    return bestIdx;
}
