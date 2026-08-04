#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ds/dsSWAP.h"

// aiFILT_ENEMY::PassDistance(const aiBRAIN*) @ 0x83220878
// ?PassDistance@aiFILT_ENEMY@@QAAXPBVaiBRAIN@@@Z
// Recompute distances, then sort the working set ascending by the dist4cmp weight.
void aiFILT_ENEMY::PassDistance(const aiBRAIN *viewer)
{
    FillDistance(viewer);

    aiCMP_EDPAIR cmp{};
    dsSWAP<aiFEE> swap{};
    arr.Sort<aiCMP_EDPAIR, dsSWAP<aiFEE> >(cmp, swap);
}
