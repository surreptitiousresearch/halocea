// aiPLANNER::ProcessUnblockNavByRemovedInst @ 0x83247018
// ?ProcessUnblockNavByRemovedInst@aiPLANNER@@IAAXPAVmsgADDR@@@Z
//
// Unregister-message handler: when the removed actor `pSend` is an entity whose animated instance
// carries the SMR (self-moving-rigid) nav feature, ask the SMR subsystem to remove that object's
// contribution to the nav mesh (unblocking waypoints/edges flagged 0x2 / 0x4).
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPHYS_SMR.h"
#include "../../headers/ws/msg/msgADDR.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h"

struct entDESC { static dsTYPE_ID TYPE_ID; }; // ?TYPE_ID@entDESC@@2VdsTYPE_ID@@A (boundary static)

void aiPLANNER::ProcessUnblockNavByRemovedInst(msgADDR *pSend)
{
    static apCOUNTER_TIME_CALL localTimeCounter("process_frame/ai/planner/ProcessUnblockNavByRemovedInst");
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> timerScope(&localTimeCounter);

    if (!pSend)
        return;

    dsTYPE_ID tid;
    if (!pSend->__vftable->TypeID(pSend, &tid)->IsDerivedFrom(entDESC::TYPE_ID))
        return;

    animINST *pInst = reinterpret_cast<entENTITY *>(pSend)->pInst;
    if (!aiPHYS_SMR::HaveSMRFeature(pInst))
        return;

    dsTSTRING<char> objName;
    objName.UnsafeInitEmpty();
    (&smr)->ProcessRemoveObjVsNav(pInst, objName, 2u, 4u);
}
