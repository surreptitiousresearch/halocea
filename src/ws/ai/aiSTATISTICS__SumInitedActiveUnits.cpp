#include "../../headers/ws/ai/aiSTATISTICS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"  // global aiPlanner
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"           // brain->st
#include "../../headers/ws/ai/aiSTATUS.h"          // aiSTATUS::gen
#include "../../headers/ws/ds/WEAK_PTR.h"

// aiSTATISTICS::SumInitedActiveUnits() const @ 0x832A9F10
// ?SumInitedActiveUnits@aiSTATISTICS@@IBAHXZ
//
// Count active-unit watchers whose tracked brain's status has the "un-inited" gen bit (0x20) clear.
// Matches the binary, which does not null-check the watcher or brain before the status read.
int aiSTATISTICS::SumInitedActiveUnits() const
{
    int count = 0;
    int n = aiPlanner->wah.arrUnits.nElem;
    for (int i = 0; i < n; ++i)
    {
        aiWATCHER_BRAIN *watcher = aiPlanner->wah.PeekActive(i);
        ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = watcher->brain.pHandle;
        aiBRAIN *brain = handle ? handle->pPtr : nullptr;
        if ((((unsigned int)brain->st.pointee->gen.val >> 5) & 1) == 0)
            ++count;
    }
    return count;
}
