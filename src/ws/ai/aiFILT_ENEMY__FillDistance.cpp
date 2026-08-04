#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiFILT_ENEMY::FillDistance(const aiBRAIN*) @ 0x83220800
// ?FillDistance@aiFILT_ENEMY@@QAAXPBVaiBRAIN@@@Z
// Recompute dist/dist4cmp on every FEE using the melee or ranged metric, selected from the viewer's
// descriptor.
void aiFILT_ENEMY::FillDistance(const aiBRAIN *viewer)
{
    // boundary: descriptor flag at *(aiBRAIN + 0x3B8) + 0x258, bit 0 selects the melee metric.
    const char *desc =
        *reinterpret_cast<const char *const *>(reinterpret_cast<const char *>(viewer) + 0x3B8);
    bool useMelee = (*reinterpret_cast<const unsigned short *>(desc + 600) & 1) != 0;

    int nElem = arr.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiFEE &fee = arr[i];
        if (useMelee)
            fee.CalculateDistanceForMelee(viewer);
        else
            fee.CalculateDistanceForRanged(viewer);
    }
}
