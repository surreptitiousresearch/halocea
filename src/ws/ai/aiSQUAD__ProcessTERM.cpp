#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiPLANNER.h"        // aiPLANNER::UnRegisterSquad
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner

// aiSQUAD::ProcessTERM @ 0x832A8040
// ?ProcessTERM@aiSQUAD@@UAAXXZ
//
// Teardown: unregister the squad from the AI planner (when one exists), then run base entity teardown.
void aiSQUAD::ProcessTERM()
{
    if (aiPlanner)
        aiPlanner->UnRegisterSquad(this);

    entENTITY::ProcessTERM();
}
