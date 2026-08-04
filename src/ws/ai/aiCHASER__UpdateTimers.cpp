// aiCHASER::UpdateTimers @ 0x8320B668
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // m3dUpdateTimeField

// Count all chaser timers down by `time`, unless AI processing is globally disabled.
void aiCHASER::UpdateTimers(float time)
{
    if (!aiDEBUG::IsOffAI())
    {
        m3dUpdateTimeField(&this->timerSawPlayer, time);
        m3dUpdateTimeField(&this->timerRecentDeath, time);
        m3dUpdateTimeField(&this->timerSparseUpPlrPos, time);
    }
}
