#include "../../headers/ws/ai/aiVISION_BATTLE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPlanner global
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/m3d/m3d_boundary.h"    // m3dClamp

// aiVISION_BATTLE::CalculateLostSightTime @ 0x83291988
// ?CalculateLostSightTime@aiVISION_BATTLE@@MBAMM@Z
//
// How long a target keeps being "seen" after it breaks line of sight, as a function of distance.
// In the planner's special mode (state bit 0x10000) a fixed short time is used — extended when the
// target is the player and currently more visible than the latest test; otherwise the time ramps
// with distance.
float aiVISION_BATTLE::CalculateLostSightTime(float dist) const
{
    float time = 0.5f;
    if (aiPlanner->state.val & 0x10000) // planner-state bit (AI_PLAST meanings are a boundary)
    {
        // decompiler shows spurious extra args on the IsPlayer call; it takes none (UBA_NXZ).
        if (viewPercent > viewLatestTest && pBrain->mind.enemy.pointee->IsPlayer())
            time = 2.5f;
        return time;
    }
    return m3dClamp(0.125f, 0.5f, 5.0f, 15.0f, dist);
}
