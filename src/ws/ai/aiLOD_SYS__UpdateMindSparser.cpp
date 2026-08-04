#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiPLANNER.h"   // aiPlanner global + wah
extern aiPLANNER *aiPlanner;   // ai08 global planner singleton
#include "../../headers/ws/ai/aiWAH_SYS.h"   // PeekActive / arrUnits
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiLOD_SYS::UpdateMindSparser @ 0x83239260
// ?UpdateMindSparser@aiLOD_SYS@@IAAXXZ
//
// Advance the round-robin "mind sparser" cursor to the next optimizable active unit and record its
// brain as the single unit permitted a full mind (think) frame this tick. Scans at most one full
// lap; leaves the permit cleared when there are no units or none are optimizable.
void aiLOD_SYS::UpdateMindSparser()
{
    int nUnits = aiPlanner->wah.arrUnits.nElem;
    if (!nUnits)
    {
        msAllowed.idx = 0;
        msAllowed.pb = nullptr;
        return;
    }
    if (nUnits <= 0)
        return;

    aiWATCHER_BRAIN *chosen = nullptr;
    int scanned = 0;
    while (true)
    {
        if (++msAllowed.idx >= nUnits)
            msAllowed.idx = 0;

        aiWATCHER_BRAIN *unitWatcher = aiPlanner->wah.PeekActive(msAllowed.idx);
        if (unitWatcher && unitWatcher->IsOptimizable())
        {
            chosen = unitWatcher;
            break;
        }
        if (++scanned >= nUnits)
            return;
    }

    msAllowed.pb = chosen->brain.pHandle->pPtr;
}
