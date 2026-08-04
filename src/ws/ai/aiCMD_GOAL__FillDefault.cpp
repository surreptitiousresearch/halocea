#include "../../headers/ws/ai/aiCMD_GOAL.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"

// m3d subsystem — global zero vector. boundary data.
extern const m3dV m3dVZero;

// aiCMD_GOAL::FillDefault(aiBRAIN_DESC*) @ 0x83175DB0
// ?FillDefault@aiCMD_GOAL@@QAAXPAVaiBRAIN_DESC@@@Z
// Reset the goal to its default state: goal + reach-check positions zeroed, reach radius 0.5,
// and the reach-distance multiplier seeded from the brain descriptor.
void aiCMD_GOAL::FillDefault(aiBRAIN_DESC *desc)
{
    posGoal = m3dVZero;
    posChkReach = m3dVZero;
    distReach = 0.5f;
    distReachMultiplier = desc->distReachMultiplier;
}
