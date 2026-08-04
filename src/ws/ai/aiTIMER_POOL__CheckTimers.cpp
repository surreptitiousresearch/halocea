#include "../../headers/ws/ai/aiTIMER_POOL.h"

// aiTIMER_POOL::CheckTimers @ 0x8317AB18
// ?CheckTimers@aiTIMER_POOL@@QAAXAAV?$dsVECTOR@VaiTIMER_DATA@aiTIMER_POOL@@$07@@@Z
//
// Reset `timerEvents`, then walk the live timers back-to-front: for every timer that has elapsed
// (timer < ~0), copy its aiTIMER_DATA payload into `timerEvents`. Cyclic timers re-arm to defVal
// (and stay in the pool); one-shot timers are erased.
void aiTIMER_POOL::CheckTimers(dsVECTOR<aiTIMER_DATA, 8> &timerEvents)
{
    timerEvents.nElem = 0;

    for (int i = this->timers.nElem - 1; i >= 0; --i)
    {
        aiTIMER &t = this->timers[i];
        bool done = t.timer < 0.000001f;
        if (done && t.isCycle)
            t.timer = t.defVal;

        if (done)
        {
            // aiTIMER derives from aiTIMER_DATA — slice the event payload into the output vector.
            timerEvents.PushBack(this->timers[i]);
            if (!this->timers[i].isCycle)
                this->timers.Erase(i, 1);
        }
    }
}
