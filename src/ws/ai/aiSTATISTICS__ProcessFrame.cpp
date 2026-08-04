#include "../../headers/ws/ai/aiSTATISTICS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"  // global aiPlanner

// aiSTATISTICS::ProcessFrame(float) @ 0x832A9EC8
// ?ProcessFrame@aiSTATISTICS@@QAAXM@Z
//
// Per-frame stats tick: refresh the "player directly seen" flag, then raise the planner state bits
// 0x100/0x200/0x400 and drop bit 0x4.
void aiSTATISTICS::ProcessFrame(float /*delta*/)
{
    UpdatePlayerDirectlySeen();
    aiPlanner->state.val |= 0x700u;
    aiPlanner->state.val &= ~4u;
}
