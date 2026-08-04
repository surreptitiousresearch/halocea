#include "../../headers/ws/ai/aiTIMER_POOL.h"

// aiTIMER_POOL::RemoveTimersByContext @ 0x83179E88
// ?RemoveTimersByContext@aiTIMER_POOL@@QAAXH@Z
//
// Remove every timer whose context tag matches `context`, scanning back-to-front so erasures do
// not disturb yet-to-be-visited indices. The compare reads the aiTIMER_DATA base context (offset
// 4), not the derived aiTIMER::context shadow (offset 0x18).
void aiTIMER_POOL::RemoveTimersByContext(int context)
{
    for (int i = timers.nElem - 1; i >= 0; --i)
    {
        if (timers[i].aiTIMER_DATA::context == context)
            timers.Erase(i, 1);
    }
}
