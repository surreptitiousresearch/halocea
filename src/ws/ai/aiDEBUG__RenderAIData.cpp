// aiDEBUG::RenderAIData @ 0x8319BC60
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// Draw every per-brain AI debug group, unless the brain is already fully spawned
// (general-state bit 2 set).
void aiDEBUG::RenderAIData()
{
    if ((((unsigned int)this->pBrain->st.pointee->gen.val >> 2) & 1) == 0)
    {
        // Reset the debug state to its low nibble with the high bit forced on.
        unsigned int state = this->state.state;
        this->state.state = state & 0xF;
        this->state.state = (state & 0xF) | 0x80000000;

        RenderCDTGroup();
        RenderCoordSysGroup();
        RenderEnemyGroup();
        RenderGoalGroup();
        RenderWeaponGroup();
        RenderHOGroup();
        RenderPercGroup();
        RenderOtherTempGroup();
    }
}
