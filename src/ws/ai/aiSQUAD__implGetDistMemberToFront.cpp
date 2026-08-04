#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/nav/navWP_SET_boundary.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"   // _m3dCheckValid

extern void _apLog(const char *fmt, ...); // engine formatted log sink. boundary.

// aiSQUAD::implGetDistMemberToFront(unsigned int, const dsTSTRING<char>&) @ 0x832A8F48
// ?implGetDistMemberToFront@aiSQUAD@@UAAMHABV?$dsTSTRING@D@@@Z  (virtual)
// Nearest distance from squad member `memberIdx` to any waypoint of the named front's WP-set.
// Returns the 2000000.0 "unreachable" sentinel on any failure.
float aiSQUAD::implGetDistMemberToFront(unsigned int memberIdx, const dsTSTRING<char> &frontName)
{
    aiCOORD_GROUP *group = GetGroupImpl();
    if (!group || !group->HaveMembers())
    {
        dsTSTRING<char> name = GetName();
        _apLog("~AI,Ssl,Error~'%s' GetDistMemberToFront(%d, \"%s\"): invalid squad",
               name.pBuffer->str, memberIdx, frontName.pBuffer->str);
        return 2000000.0f;
    }

    dsTSTRING<char> errReason;   // empty; GetWPSet writes a failure reason into it
    navWP_SET *wpSet = GetWPSet(group, frontName, errReason);
    if (!wpSet)
    {
        dsTSTRING<char> name = GetName();
        _apLog("~AI,Ssl,Error~'%s' GetDistMemberToFront(%d, \"%s\"): %s",
               name.pBuffer->str, memberIdx, frontName.pBuffer->str, errReason.pBuffer->str);
        return 2000000.0f;
    }

    iaIACTOR *member = group->GetMember(memberIdx);

    // vtbl slot 0x1C0 (index 112): iaIACTOR::GetCenter(m3dV*) — 2-level boundary dispatch.
    m3dV center;
    void **memberVtbl = *reinterpret_cast<void ***>(member);
    typedef void (*GetCenterFn)(iaIACTOR *, m3dV *);
    reinterpret_cast<GetCenterFn>(memberVtbl[112])(member, &center);

    float bestSq = 2000000.0f;
    int nWp = wpSet->GetNWP();
    for (int i = 0; i < nWp; ++i)
    {
        float d2 = wpSet->GetWP(i)->Dist_2(&center);
        if (d2 < bestSq)
            bestSq = d2;
    }

    _m3dCheckValid(bestSq);
    return __fsqrts(bestSq);
}
