#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiWATCHER_PLAYER_fsm_boundary.h" // propFSM + fsmSTATE pointees
#include "../../headers/ws/fsm/fsmEVENT_HANDLER.h"
#include "../../headers/ws/fsm/fsmAIMING_EVENT.h"              // fsmEVENT_PARAM_BASE
#include "../../headers/ws/ds/dsSTRID.h"

// aiBODY::SendFsmEventByName(const dsSTRID&, const fsmEVENT_PARAM_BASE&) @ 0x832AC4E0
// ?SendFsmEventByName@aiBODY@@UAA_NABVdsSTRID@@ABUfsmEVENT_PARAM_BASE@@@Z
//
// Named-event variant carrying a parameter block. Routed to the held FSM if suspended, else the
// active FSM, via the handler's non-virtual SendEventByName.
bool aiBODY::SendFsmEventByName(const dsSTRID &idEvent, const fsmEVENT_PARAM_BASE &par)
{
    fsmEVENT_HANDLER *handler = bodyFsm->fsmHld.pointee
        ? static_cast<fsmEVENT_HANDLER *>(bodyFsm->fsmHld.pointee)
        : static_cast<fsmEVENT_HANDLER *>(bodyFsm->fsm.pointee);
    handler->SendEventByName(idEvent, par);
    return true;
}
