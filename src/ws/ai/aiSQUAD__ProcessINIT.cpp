#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiPLANNER.h"        // aiPLANNER::RegisterSquad
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner

// aiSQUAD::ProcessINIT @ 0x832A7FC8
// ?ProcessINIT@aiSQUAD@@UAAHPAX@Z
//
// Per-spawn init: run the base entity init; abort if it fails or no animated instance was created.
// Otherwise register the squad with the AI planner (when one exists). Returns nonzero on success.
int aiSQUAD::ProcessINIT(void *pInfo)
{
    if (!entENTITY::ProcessINIT(pInfo) || !pInst)
        return 0;

    if (aiPlanner)
        aiPlanner->RegisterSquad(this);

    return 1;
}
