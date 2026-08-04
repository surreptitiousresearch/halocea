#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/fsm/fsmWPN_SPRD_INFO.h"

// aiWEAPON::OnSpreadRequested @ 0x83292DA0
// ?OnSpreadRequested@aiWEAPON@@IAAXPAVfsmWPN_SPRD_INFO@@@Z
//
// Answer a weapon FSM's spread request with the module's current spread scale (unless dead).
void aiWEAPON::OnSpreadRequested(fsmWPN_SPRD_INFO *par)
{
    if (!pBrain->IsDead())
        par->spreadScale = spreadScale;
}
