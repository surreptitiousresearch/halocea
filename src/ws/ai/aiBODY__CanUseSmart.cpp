#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/smt/smtSMART_OBJ_BASE.h"

// 0x832ABD88 ?CanUseSmart@aiBODY@@UAA_N...
// The body may use `smtObj` for entry `action` when the object's usage condition passes for this
// body's FSM and the entry action is currently available (query flags 4 = "peek availability").
bool aiBODY::CanUseSmart(dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > &smtObj,
                         const dsPAIR<dsSTRID, dsSTRID> &action)
{
    if (!smtObj.pointee->CheckActionUsingCond(this->bodyFsm))
        return false;

    return smtObj.pointee->IsActionAvailable(&action, this->bodyFsm, nullptr, 4u);
}
