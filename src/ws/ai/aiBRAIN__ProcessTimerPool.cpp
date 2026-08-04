#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/bhvBEHAV.h"
#include "../../headers/ws/ai/aiTIMER_POOL.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/apCL.h"

// aiBRAIN::ProcessTimerPool() @ 0x832AEBF8
// ?ProcessTimerPool@aiBRAIN@@QAAXXZ
//
// Collect every timer that fired this frame and route each event: brain-context timers
// (context == 0x01000000) go to the brain's OnTimer; all others go to the currently-selected
// behaviour's OnTimer (unless it has already finished).
void aiBRAIN::ProcessTimerPool()
{
    apCL cl = { "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_frame.cpp", 149 };
    dsVECTOR<aiTIMER_POOL::aiTIMER_DATA, 8> firedEvents(cl);

    timers.CheckTimers(firedEvents);

    int nElem = firedEvents.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        if (firedEvents[i].context == 0x1000000)
        {
            OnTimer(firedEvents[i].id);
        }
        else
        {
            bhvBEHAV *bhv = mind.best.pointee->FindBHV();
            if (bhv && !bhv->state.Test(BHV_ST_FINISHED))
                bhv->OnTimer(firedEvents[i].id);
        }
    }
}
