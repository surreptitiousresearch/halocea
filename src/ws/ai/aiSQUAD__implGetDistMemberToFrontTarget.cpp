#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/nav/navWP_SET_boundary.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/m3d/m3dV.h"

extern aiPLANNER *aiPlanner;             // ?aiPlanner@@3PAVaiPLANNER@@A — global AI planner
extern void _apLog(const char *fmt, ...); // engine formatted log sink. boundary.

// aiSQUAD::implGetDistMemberToFrontTarget(unsigned int, const dsTSTRING<char>&) @ 0x832A9210
// ?implGetDistMemberToFrontTarget@aiSQUAD@@UAAMHABV?$dsTSTRING@D@@@Z  (virtual)
// Distance from squad member `memberIdx` to the coordinator-selected target waypoint of the named
// front's WP-set. Returns the 2000000.0 "unreachable" sentinel on any failure.
float aiSQUAD::implGetDistMemberToFrontTarget(unsigned int memberIdx, const dsTSTRING<char> &frontName)
{
    aiCOORD_GROUP *group = GetGroupImpl();
    if (!group || !group->HaveMembers())
    {
        dsTSTRING<char> name = GetName();
        _apLog("~AI,Ssl,Error~'%s' GetDistMemberToFrontTarget(%d, \"%s\"): invalid squad",
               name.pBuffer->str, memberIdx, frontName.pBuffer->str);
        return 2000000.0f;
    }

    dsTSTRING<char> errReason;   // empty; GetWPSet writes a failure reason into it
    navWP_SET *wpSet = GetWPSet(group, frontName, errReason);
    if (!wpSet)
    {
        dsTSTRING<char> name = GetName();
        _apLog("~AI,Ssl,Error~'%s' GetDistMemberToFrontTarget(%d, \"%s\"): %s",
               name.pBuffer->str, memberIdx, frontName.pBuffer->str, errReason.pBuffer->str);
        return 2000000.0f;
    }

    iaIACTOR *member = group->GetMember(memberIdx);

    // vtbl slot 0x1C0 (index 112): iaIACTOR::GetCenter(m3dV*) — 2-level boundary dispatch.
    m3dV center;
    void **memberVtbl = *reinterpret_cast<void ***>(member);
    typedef void (*GetCenterFn)(iaIACTOR *, m3dV *);
    reinterpret_cast<GetCenterFn>(memberVtbl[112])(member, &center);

    // Coordinator picks the member's assigned result waypoint (embedded aiCOORDINATOR @ +0x184).
    aiCOORDINATOR *coordinator = (&aiPlanner->coordinator);
    short wpId = coordinator->GetResultWP(reinterpret_cast<aiBRAIN *>(member));

    if ((wpId & 0x8000) != 0)              // 0xFFFF sentinel = no result
        return 2000000.0f;
    if (!wpSet->IsContainWP_ID(wpId))
        return 2000000.0f;

    navWP *targetWp = aiPlanner->GetWP(static_cast<unsigned short>(wpId));
    if (!targetWp)
    {
        dsTSTRING<char> name = GetName();
        _apLog("~AI,Ssl,Error~'%s' GetDistMemberToFrontTarget(%d, \"%s\"): invalid target wp",
               name.pBuffer->str, memberIdx, frontName.pBuffer->str);
        return 2000000.0f;
    }

    return targetWp->Dist(&center);
}
