// aiMIND::ForcedClear @ 0x8322BC48
// ?ForcedClear@aiMIND@@UAAXXZ
//
// Force-clear the mind during teardown: clear the best-behaviour and goal modules, zero the tracked
// enemy, and reset the status enemy sub-block.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"

void aiMIND::ForcedClear()
{
    best.pointee->ForcedClear();
    goal.pointee->ForcedClear();
    ZeroEnemy();

    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);
    brain->st.pointee->enemy.Reset();
}
