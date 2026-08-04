#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner

// aiMIND::CalcKeepDefCDTReqTime @ 0x832290E8
// ?CalcKeepDefCDTReqTime@aiMIND@@IBAMXZ  (protected, const)
//
// How long (seconds) to keep this unit's default combat-decision-tree requests alive. Zero unless
// the unit is itself aggressive (aiSTATUS::gen bit 4). Otherwise it scales with the number of
// aggressive squad-mates currently active: (aggressiveCount + 10) * 0.1.
float aiMIND::CalcKeepDefCDTReqTime() const
{
    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    bool selfAggressive = (((unsigned int)brain->st.pointee->gen.val >> 4) & 1) != 0;
    if (!selfAggressive || aiPlanner == nullptr)
        return 0.0f;

    int aggressiveCount = 0;
    int n = aiPlanner->wah.arrUnits.nElem;
    for (int i = 0; i < n; ++i)
    {
        aiWATCHER_BRAIN *w = aiPlanner->wah.PeekActive(i);
        if (w->IsBrain())
        {
            aiBRAIN *other = w->brain.pHandle ? w->brain.pHandle->pPtr : nullptr;
            if ((((unsigned int)other->st.pointee->gen.val >> 4) & 1) != 0)
                ++aggressiveCount;
        }
    }

    return (float)(aggressiveCount + 10) * 0.1f;
}
