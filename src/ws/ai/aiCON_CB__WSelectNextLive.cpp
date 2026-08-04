#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiCON_CB::WSelectNextLive @ 0x83181218
// ?WSelectNextLive@aiCON_CB@@IAAXXZ
//
// Advance the debug "watchee" to the next live (active-list) AI unit after the currently-selected
// one, wrapping to the first. Writes the chosen brain (or null) into the global _pBrainDebugWatchee.
void aiCON_CB::WSelectNextLive()
{
    if (!aiPlanner)
        return;

    int coLive = 0, coDead = 0, idxLive = -1, idxDead = -1, extra = 0;
    this->GetAICount(&coLive, &coDead, &idxLive, &idxDead, &extra);

    if (coLive >= 1)
    {
        int idx;
        bool valid = true;
        if (idxLive + 1 < coLive)
        {
            idx = idxLive + 1;
            if (idxLive + 1 < 0)
                valid = false;
        }
        else
        {
            idx = 0;
        }

        if (valid)
        {
            aiWATCHER_BRAIN *wb = aiPlanner->wah.PeekActive(idx);
            if (wb)
            {
                aiBRAIN **pPtr = &wb->brain.pHandle->pPtr;
                if (pPtr)
                {
                    _pBrainDebugWatchee = *pPtr;
                    return;
                }
            }
        }
    }

    _pBrainDebugWatchee = nullptr;
}
