#include "../../headers/ws/ai/aiPHYS_SMR.h"
#include "../../headers/ws/anim/animINST_boundary.h"   // animINST::pPhysSystem / name
#include "../../headers/ws/phys/physSYSTEM_BASE.h"      // physSYSTEM_BASE::DESC::state

// Shared empty-string literal used as the null-name fallback.
extern char byte_8200155A[];

// aiPHYS_SMR::IsAcceptProcessing @ 0x83238980
// ?IsAcceptProcessing@aiPHYS_SMR@@IAA_NPAVanimINST@@@Z
//
// Should this anim instance be processed by the SMR nav-blocking feature? Rejects when: there is no
// instance; the grace timer is still running and SSL hasn't force-enabled; the instance's physics
// system does not participate in nav blocking (DESC state bit 10) and SSL hasn't force-enabled; or
// the instance has an empty name.
bool aiPHYS_SMR::IsAcceptProcessing(animINST *pInst)
{
    if (!pInst || (!isEnabledBySSL && timerIgnore > 0.000001f))
        return false;

    physSYSTEM_BASE *pPhysSystem = pInst->pPhysSystem;
    bool participatesInNav = false;
    if (pPhysSystem)
        participatesInNav = (pPhysSystem->pDesc->state.state >> 10) & 1;

    if (!participatesInNav && !isEnabledBySSL)
        return false;

    const char *name = pInst->name;
    if (!name)
        name = byte_8200155A;
    return *name != 0;
}
