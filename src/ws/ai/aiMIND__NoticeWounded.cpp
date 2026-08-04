#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_ssl_boundary.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiPERC_BASE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner
#include "../../headers/ws/dmg/dmgDAMAGE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/WEAK_PTR.h"

// aiMIND::NoticeWounded @ 0x83229578
// ?NoticeWounded@aiMIND@@QAAXABVdmgDAMAGE@@@Z
//
// Propagate a wound event: fire the SSL "NoticeWounded" script event, then notify the
// best-behaviour and perception modules, and finally the planner's squad coordinator.
void aiMIND::NoticeWounded(const dmgDAMAGE &dmg)
{
    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    // RAII: the returned sslERROR temporary releases itself at end of statement.
    brain->sslObject.HandleEvent(aiBRAIN_DESC_SslEvNoticeWounded(brain->spDesc.pointee), 0, nullptr);

    best.pointee->NoticeWounded(dmg.amount);

    ds::WEAK_PTR_HANDLE<entENTITY> *shooterHandle = dmg.params.entShooter.pHandle;
    entENTITY *shooter = shooterHandle ? shooterHandle->pPtr : nullptr;
    perc.pointee->NoticeWounded(shooter, dmg.amount);

    // aiPLANNER.coordinator is a byte-accurate opaque aiCOORDINATOR block (its body is the next
    // frontier); reach its NoticeWounded entry point by reinterpreting the block.
    (&aiPlanner->coordinator)->NoticeWounded(brain, dmg.amount);
}
