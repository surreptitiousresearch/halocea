#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog

// aiBRAIN::implKillTimer @ 0x83179F98
// ?implKillTimer@aiBRAIN@@UAAXH@Z
//
// Remove the brain's timer `timerID`. If no such timer existed, log a diagnostic naming the unit.
void aiBRAIN::implKillTimer(int timerID)
{
    if (!timers.RemoveTimerByID(timerID))
        _apLog("~AI,Ssl,Error~'%s' attempted to kill absent timer. id = %d",
               names.nameInst, timerID);
}
