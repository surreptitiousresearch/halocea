#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiWATCHER_PLAYER_fsm_boundary.h" // propFSM + fsmCB_HANDLER_dsSTRID + fsmCB_POS_DIR_PAR
#include "../../headers/ws/fsm/fsmFSM_EVENT.h"

// aiBODY::Transport(fsmCB_POS_DIR_PAR&) @ 0x832AC928
// ?Transport@aiBODY@@UAAXAAVfsmCB_POS_DIR_PAR@@@Z
//
// Fire the body FSM's "reset" callback, forwarding the position/direction transport parameter to
// whichever handler is registered for it.
void aiBODY::Transport(fsmCB_POS_DIR_PAR &par)
{
    bodyFsm->eventHandler.ActivateCB(fsmFSM_EVENT::reset_(), &par);
}
