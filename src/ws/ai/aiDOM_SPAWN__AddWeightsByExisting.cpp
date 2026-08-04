#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiDOM_SPAWN::AddWeightsByExisting @ 0x83265718  ?AddWeightsByExisting@aiDOM_SPAWN@@AAAXXZ
// Bias spawn-point weights toward every currently-active AI unit (inverse-square, vertical doubled).
void aiDOM_SPAWN::AddWeightsByExisting()
{
    if (!aiPlanner)
        return;

    for (int i = 0; i < aiPlanner->wah.arrUnits.nElem; ++i)
    {
        aiWATCHER_BRAIN *unit = aiPlanner->wah.PeekActive(i);
        m3dV bodyPos;
        m3dV *bp = unit->GetBodyPos(&bodyPos);

        for (int j = 0; j < this->pointsList.nElem; ++j)
        {
            dipPOINT &pt = this->pointsList[j];
            float dx = bp->x - pt.pos.x;
            float dy = (bp->y - pt.pos.y) * 2.0f;
            float dz = bp->z - pt.pos.z;
            pt.weight = 1.0f / ((dx * dx) + ((dz * dz) + (dy * dy)) + 0.1f) + pt.weight;
        }
    }
}
