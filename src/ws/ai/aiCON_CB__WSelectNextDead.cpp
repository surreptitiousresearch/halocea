#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/msg/msgSYSTEM.h"
#include "../../headers/ws/msg/msgTYPE_ITER.h"
#include "../../headers/ws/msg/msgADDR.h"

// aiCON_CB::WSelectNextDead @ 0x831812F0
// ?WSelectNextDead@aiCON_CB@@IAAXXZ
//
// Advance the debug "watchee" to the next dead AI unit after the currently-selected one, wrapping
// to the first. Dead units are enumerated in message-system iteration order; a unit is "dead" here
// when its msgADDR reports a non-zero suspend mask. The target ordinal is the current dead index+1
// (wrapping). Writes the chosen brain (or null) into the global _pBrainDebugWatchee.
void aiCON_CB::WSelectNextDead()
{
    int coLive = 0, coDead = 0, idxLive = -1, idxDead = -1, extra = 0;
    this->GetAICount(&coLive, &coDead, &idxLive, &idxDead, &extra);

    if (coDead < 1)
    {
        _pBrainDebugWatchee = nullptr;
        return;
    }

    int target;
    if (idxDead + 1 >= coDead)
    {
        target = 0;
    }
    else
    {
        target = idxDead + 1;
        if (idxDead + 1 < 0)
        {
            _pBrainDebugWatchee = nullptr;
            return;
        }
    }

    int suspendedSeen = 0;
    msgTYPE_ITER<aiBRAIN> it(msgSystem.Front());
    aiBRAIN *node = it.Get();
    if (!node)
        return;

    while (true)
    {
        aiBRAIN *cur = node;
        msgADDR *addr = node; // aiBRAIN derives from msgADDR (via iaIACTOR)
        if (addr->__vftable->GetSuspendMask(addr))
        {
            if (suspendedSeen == target)
            {
                _pBrainDebugWatchee = cur;
                return;
            }
            ++suspendedSeen;
        }
        ++it;
        node = it.Get();
        if (!node)
            return;
    }
}
