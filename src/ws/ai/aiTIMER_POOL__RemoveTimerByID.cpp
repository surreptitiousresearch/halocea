#include "../../headers/ws/ai/aiTIMER_POOL.h"

// aiTIMER_POOL::RemoveTimerByID @ 0x83179E18
// ?RemoveTimerByID@aiTIMER_POOL@@QAA_NH@Z
//
// Remove the first timer whose identity id matches `id`; return true if one was found. The compare
// reads the aiTIMER_DATA base id (offset 0), not the derived aiTIMER::id shadow (offset 8).
bool aiTIMER_POOL::RemoveTimerByID(int id)
{
    int nElem = timers.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        if (timers[i].aiTIMER_DATA::id == id)
        {
            timers.Erase(i, 1);
            return true;
        }
    }
    return false;
}
