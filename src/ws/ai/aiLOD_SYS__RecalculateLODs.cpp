#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiPLANNER.h"        // aiPlanner global + wah
extern aiPLANNER *aiPlanner;   // ai08 global planner singleton
#include "../../headers/ws/ai/aiWAH_SYS.h"        // PeekActive / PeekPlayer / arrUnits / arrPlayers
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// aiLOD_SYS::RecalculateLODs @ 0x83239110
// ?RecalculateLODs@aiLOD_SYS@@IAAXXZ
//
// Recompute every live AI unit's LOD value as the minimum CalcLODValue over all active player
// watchers (seed 888 = "no player in range"), storing the truncated result back into the unit's
// aiSTATUS LOD sub-block. Dead units are skipped.
void aiLOD_SYS::RecalculateLODs()
{
    if (!aiPlanner)
        return;

    int nUnits = aiPlanner->wah.arrUnits.nElem;
    int nPlayers = aiPlanner->wah.arrPlayers.nElem;
    if (nUnits <= 0)
        return;

    for (int u = 0; u < nUnits; ++u)
    {
        aiWATCHER_BRAIN *unitWatcher = aiPlanner->wah.PeekActive(u);
        if (!unitWatcher || unitWatcher->IsDead())
            continue;

        ds::WEAK_PTR_HANDLE<aiBRAIN> *pHandle = unitWatcher->brain.pHandle;
        aiBRAIN *pBrain = pHandle ? pHandle->pPtr : nullptr;
        aiSTATUS::LOD *lod = &pBrain->st.pointee->lod;
        lod->Reset();

        float best = 888.0f;
        for (int p = 0; p < nPlayers; ++p)
        {
            aiWATCHER *player = aiPlanner->wah.PeekPlayer(p);
            if (player)
            {
                float value = CalcLODValue(unitWatcher, player);
                if (value < best)
                    best = value;
            }
        }
        lod->value = (int)best;
    }
}
