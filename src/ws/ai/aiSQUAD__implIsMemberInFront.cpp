#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiDOM_NAV_boundary.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/m3d/m3dV.h"

extern aiPLANNER *aiPlanner;             // ?aiPlanner@@3PAVaiPLANNER@@A — global AI planner
extern void _apLog(const char *fmt, ...); // engine formatted log sink. boundary.

// aiSQUAD::implIsMemberInFront(unsigned int, const dsTSTRING<char>&) @ 0x832A8410
// ?implIsMemberInFront@aiSQUAD@@UAA_NHABV?$dsTSTRING@D@@@Z  (virtual)
// True when squad member `memberIdx` currently stands inside the named front's nav domain.
bool aiSQUAD::implIsMemberInFront(unsigned int memberIdx, const dsTSTRING<char> &frontName)
{
    aiCOORD_GROUP *group = GetGroupImpl();
    if (group && group->HaveMembers())
    {
        aiDOM_NAV *domain = aiPlanner->GetNavManager()->GetNavDomain(frontName);
        if (domain)
        {
            iaIACTOR *member = group->GetMember(memberIdx);

            // vtbl slot 0x1C0 (index 112): iaIACTOR::GetCenter(m3dV*) — 2-level boundary dispatch.
            m3dV center;
            void **memberVtbl = *reinterpret_cast<void ***>(member);
            typedef void (*GetCenterFn)(iaIACTOR *, m3dV *);
            reinterpret_cast<GetCenterFn>(memberVtbl[112])(member, &center);

            // scnDOMAIN sub-object at aiDOM_NAV+0xC8; IsBelongPoint is scnDOMAIN vtbl slot 0x0C
            // (index 3) — ?IsBelongPoint@scnDOMAIN@@UBAHPBUm3dV@@PAG@Z. boundary dispatch.
            unsigned char *scnDomain = reinterpret_cast<unsigned char *>(domain) + 0xC8;
            void **domainVtbl = *reinterpret_cast<void ***>(scnDomain);
            typedef int (*IsBelongPointFn)(void *, const m3dV *, unsigned short *);
            return reinterpret_cast<IsBelongPointFn>(domainVtbl[3])(scnDomain, &center, nullptr) != 0;
        }

        dsTSTRING<char> name = GetName();
        _apLog("~AI,Ssl,Error~'%s' IsMemberInFront(%d, \"%s\"): front domain not found",
               name.pBuffer->str, memberIdx, frontName.pBuffer->str);
    }
    else
    {
        dsTSTRING<char> name = GetName();
        _apLog("~AI,Ssl,Error~'%s' IsMemberInFront(%d, \"%s\"): invalid squad",
               name.pBuffer->str, memberIdx, frontName.pBuffer->str);
    }
    return false;
}
