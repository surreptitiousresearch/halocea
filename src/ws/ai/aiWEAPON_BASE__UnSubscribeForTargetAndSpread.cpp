#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/ai/aiWEAPON_BASE_fsm_boundary.h" // fsmWEAPON_EVENT ids + propFSM cast
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"

// aiWEAPON_BASE::UnSubscribeForTargetAndSpread @ 0x83295C88
// ?UnSubscribeForTargetAndSpread@aiWEAPON_BASE@@UAAXPAVpropWPN_AI_USABLE@@@Z  (virtual)
//
// Detach this weapon module from the controller's weapon-FSM target and spread request events.
void aiWEAPON_BASE::UnSubscribeForTargetAndSpread(propWPN_AI_USABLE *ctrl)
{
    // The controller's wpnWEAPON is a propFSM; its event handler lives at propFSM::eventHandler.
    propFSM *fsm = reinterpret_cast<propFSM *>(ctrl->weapon());
    fsm->eventHandler.UnregisterCB(fsmWEAPON_EVENT::request_tgt_(), this);

    fsm = reinterpret_cast<propFSM *>(ctrl->weapon());
    fsm->eventHandler.UnregisterCB(fsmWEAPON_EVENT::request_sprd_(), this);
}
