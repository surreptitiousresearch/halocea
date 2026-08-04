#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/fsm/fsmWPN_TGT_INFO.h"

// aiWEAPON::OnTgtRequested @ 0x83292D40
// ?OnTgtRequested@aiWEAPON@@IAAXPAVfsmWPN_TGT_INFO@@@Z
//
// Answer a weapon FSM's target request with the brain's current weapon-target position (unless the
// brain is dead, in which case the request is left untouched).
void aiWEAPON::OnTgtRequested(fsmWPN_TGT_INFO *par)
{
    if (!pBrain->IsDead())
        par->Set(&pBrain->cmd.posWpnTgt);
}
