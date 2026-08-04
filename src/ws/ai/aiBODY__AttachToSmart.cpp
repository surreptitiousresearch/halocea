#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiWATCHER_PLAYER_fsm_boundary.h" // propFSM (bodyFsm)
#include "../../headers/ws/smt/smtSMART_OBJ_BASE.h"            // smtSMART_OBJ_BASE::Attach
#include "../../headers/ws/ap/apCOUNTER_TIME.h"
#include "../../headers/ws/ds/dsPAIR.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiBODY::AttachToSmart(dsSHARED_PTR<smtSMART_OBJ_BASE>&, const dsPAIR<dsSTRID,dsSTRID>*, bool) @ 0x832ACB20
// ?AttachToSmart@aiBODY@@UAA_NAAV?$dsSHARED_PTR@VsmtSMART_OBJ_BASE@@$0A@V?$Deleter@VsmtSMART_OBJ_BASE@@@@@@PBV?$dsPAIR@VdsSTRID@@V1@@@_N@Z
//
// Attach this body's FSM to smart object `smtObj` at entry action `action`. No-op (returns false)
// when the action's target id is invalid; otherwise issue the attach under a profiled scope.
bool aiBODY::AttachToSmart(dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > &smtObj,
                           const dsPAIR<dsSTRID, dsSTRID> *action, bool isStreamingWait)
{
    if (!action->val.IsValid())
        return false;

    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/attach_smart", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> timeScope(&localTimeCounter);

    smtObj.pointee->Attach(bodyFsm, action, isStreamingWait);
    return true;
}
