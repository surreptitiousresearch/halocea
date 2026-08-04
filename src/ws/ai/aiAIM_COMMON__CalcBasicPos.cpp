#include "../../headers/ws/ai/aiAIM_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ent/entTRACKER.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiAIM_COMMON::CalcBasicPos @ 0x83299D58
// ?CalcBasicPos@aiAIM_COMMON@@MAA_NAAUm3dV@@@Z  (protected, returns bool)
//
// Compute the basic aim position into `newPos`. When no aim is assigned, or the assigned tracker
// resolves to the mind's tracked enemy, aim at the tracker center (or, while frozen, hold the last
// frozen position). Returns whether a position was produced.
bool aiAIM_COMMON::CalcBasicPos(m3dV &newPos)
{
    if (!IsGiven() || pBrain->mind.enemy.pointee->Is(trkAim.holder.GetEnt()))
    {
        if (isFrozen)
        {
            newPos = posToFreeze;
            return true;
        }
        return false;
    }

    trkAim.GetCenter(&newPos);
    return true;
}
