#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/msg/msgSYSTEM.h"
#include "../../headers/ws/msg/msgTYPE_ITER.h"
#include "../../headers/ws/msg/msgADDR.h"

// aiBID behaviour ids read here (see aiWATCHER_BRAIN_boundaries.h for the full enum).
enum { AI_BID_HO = 12, AI_BID_ZERO_HO = 32 };

// aiCON_CB::GetAICount @ 0x83180FB0
// ?GetAICount@aiCON_CB@@IAAXAAH0000@Z
//
// Tally the planner's live/dead unit counts, the count of units currently in a hideout behaviour
// (`pExtra`), and locate the currently-inspected watchee's index — in the live-unit list when it is
// alive, or in the message-system iteration order (counting suspended brains) when it is dead.
void aiCON_CB::GetAICount(int *pLive, int *pDeadUnits, int *pIdxLive, int *pIdxDead, int *pExtra)
{
    if (!aiPlanner)
        return;

    *pLive = aiPlanner->wah.arrUnits.nElem;
    *pDeadUnits = aiPlanner->wah.GetAllUnitsCnt() - *pLive;
    *pExtra = 0;
    *pIdxLive = -1;
    *pIdxDead = -1;

    for (int i = 0; i < *pLive; ++i)
    {
        aiWATCHER_BRAIN *wb = aiPlanner->wah.PeekActive(i);
        ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = wb->brain.pHandle;
        aiBRAIN *brain = handle ? handle->pPtr : nullptr;
        aiBEST_BASE *best = brain->mind.best.pointee;
        if (best->HasBHV(AI_BID_HO) || best->HasBHV(AI_BID_ZERO_HO))
            ++*pExtra;
    }

    ds::WEAK_PTR_HANDLE<aiBRAIN> *whHandle = this->pCurrentWatchee.pHandle;
    if (!whHandle)
        return;
    aiBRAIN *watchee = whHandle->pPtr;
    if (!watchee)
        return;

    // A watchee counts as "live" (index in the active list) when it isn't dead, or when its
    // die-state bit 1 is set; otherwise it is a "dead" watchee (index in the msg iteration order).
    bool liveBranch;
    if (!watchee->IsDead())
    {
        liveBranch = true;
    }
    else
    {
        ds::WEAK_PTR_HANDLE<aiBRAIN> *h2 = this->pCurrentWatchee.pHandle;
        aiBRAIN *w2 = h2 ? h2->pPtr : nullptr;
        liveBranch = (((unsigned int)w2->st.pointee->die.val >> 1) & 1) != 0;
    }

    if (liveBranch)
    {
        *pIdxLive = 0;
        if (*pLive > 0)
        {
            int next;
            do
            {
                aiWATCHER_BRAIN *wb = aiPlanner->wah.PeekActive(*pIdxLive);
                if (wb && wb->Is(watchee))
                    break;
                next = *pIdxLive + 1;
                *pIdxLive = next;
            } while (next < *pLive);
        }
    }
    else
    {
        *pIdxDead = 0;
        msgTYPE_ITER<aiBRAIN> it(msgSystem.Front());
        for (aiBRAIN *cur = it.Get(); cur; cur = it.Get())
        {
            if (cur == watchee)
                break;
            msgADDR *addr = cur; // aiBRAIN derives from msgADDR (via iaIACTOR)
            if (addr->__vftable->GetSuspendMask(addr))
                ++*pIdxDead;
            ++it;
        }
        if (*pIdxDead > *pDeadUnits)
            *pIdxDead = 0;
    }
}
