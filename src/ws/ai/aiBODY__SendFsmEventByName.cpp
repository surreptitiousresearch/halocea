#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiWATCHER_PLAYER_fsm_boundary.h" // propFSM + fsmSTATE_MANAGER/HLD pointees
#include "../../headers/ws/fsm/fsmEVENT_HANDLER.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiBODY::SendFsmEventByName(const dsSTRID&) @ 0x832AC478
// ?SendFsmEventByName@aiBODY@@UAA_NABVdsSTRID@@@Z
//
// Route a parameterless named event to the body FSM: the suspended (held) FSM if one exists, else
// the active FSM. Dispatched through the handler's virtual "post event id" slot (vtbl slot 1).
bool aiBODY::SendFsmEventByName(const dsSTRID &idEvent)
{
    fsmEVENT_HANDLER *handler = bodyFsm->fsmHld.pointee
        ? static_cast<fsmEVENT_HANDLER *>(bodyFsm->fsmHld.pointee)
        : static_cast<fsmEVENT_HANDLER *>(bodyFsm->fsm.pointee);
    handler->PostEventById(&idEvent);
    return true;
}
