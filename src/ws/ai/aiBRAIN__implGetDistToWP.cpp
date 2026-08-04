// aiBRAIN::implGetDistToWP @ 0x832B1BB8
// ?implGetDistToWP@aiBRAIN@@UAAMABV?$dsTSTRING@D@@@Z  (virtual)
//
// SSL "GetDistToWp(name)" body: the nav-space distance from the brain's body to waypoint `nameWP`.
// Returns the sentinel 2000000.0 when the call is gated by the common SSL back-off, when the brain
// owns no nav system, or when no waypoint of that name exists.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h"   // _apLog
#include "../../headers/ws/ai/aiPLANNER.h"          // GetNavManager
#include "../../headers/ws/ai/aiCON_CB_globals.h"   // aiPlanner
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/m3d/m3dV.h"

float aiBRAIN::implGetDistToWP(const dsTSTRING<char> &nameWP)
{
    dsTSTRING<char> callName("GetDistToObj()");
    if (this->IsSSLCommonBackOff(callName, dsFLAGS<SSL_COMMON_BACKOFF, int>{0}) ||
        !this->GetNavSystem())
        return 2000000.0f;

    // a8_goal.cpp: the nav manager singleton is asserted non-null in the original (STRONG_ASSERT).
    navMANAGER *navMan = aiPlanner->GetNavManager();
    navWP *wp = navMan->FindWP(nameWP.pBuffer->str);
    if (!wp)
    {
        _apLog("~AI,Ssl,Error~'%s': GetDistToWp('%s') no wp with this name",
               this->names.nameInst, nameWP.pBuffer->str);
        return 2000000.0f;
    }

    m3dV bodyPos = this->GetBodyPosNav();
    return wp->Dist(&bodyPos);
}
