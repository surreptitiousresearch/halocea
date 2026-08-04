#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/ent/entENTITY.h"
// entENTITY.h already pulls the canonical animINST (with GetPos); the boundary slice would
// redefine it (C2011), so it is intentionally not included here.
#include "../../headers/ws/ds/dsVECTOR.h"

// aiDOM_SPAWN::AddWeightsByPlayers @ 0x832658A8  ?AddWeightsByPlayers@aiDOM_SPAWN@@AAAXXZ
// Bias spawn-point weights toward every player (inverse-square, vertical doubled, 5x strength).
// Skipped when the desc's "ignore players" flag (desc[49]) is set.
void aiDOM_SPAWN::AddWeightsByPlayers()
{
    if (this->spDesc.pointee[49].__vftable)
        return;

    for (int i = 0; i < gsGetPlayerCount(); ++i)
    {
        entENTITY *player = gsGetPlayer(i);
        m3dV playerPos;
        player->pInst->GetPos(&playerPos);

        for (int j = 0; j < this->pointsList.nElem; ++j)
        {
            dipPOINT &pt = this->pointsList[j];
            float dx = playerPos.x - pt.pos.x;
            float dy = (playerPos.y - pt.pos.y) * 2.0f;
            float dz = playerPos.z - pt.pos.z;
            pt.weight = 5.0f / ((dx * dx) + ((dz * dz) + (dy * dy)) + 0.1f) + pt.weight;
        }
    }
}
