#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/ent/entENTITY.h"
// entENTITY.h supplies the canonical animINST (with GetPos); the boundary slice would redefine it.

// aiDOM_SPAWN::GetSpawnPosDir @ 0x83265B68  ?GetSpawnPosDir@aiDOM_SPAWN@@UAAXPAUm3dV@@0@Z
// Pick a spawn position, then aim `dir` from that position toward the nearest player (normalised).
// With no players present, aim straight up (+Z).
void aiDOM_SPAWN::GetSpawnPosDir(m3dV *pos, m3dV *dir)
{
    this->SelectSpawnPos(pos);

    if (gsGetPlayerCount())
    {
        float minDist = 2000000.0f;
        m3dV nearest = m3dVZero;
        int count = gsGetPlayerCount();
        for (int i = 0; i < count; ++i)
        {
            entENTITY *player = gsGetPlayer(i);
            m3dV playerPos;
            player->pInst->GetPos(&playerPos);
            float d = m3dDist(pos, &playerPos);
            if (d < minDist)
            {
                minDist = d;
                nearest = playerPos;
            }
        }
        _m3dCheckValid(&nearest);
        _m3dCheckValid(pos);
        dir->x = nearest.x - pos->x;
        dir->y = nearest.y - pos->y;
        dir->z = nearest.z - pos->z;
        m3dNormalize(dir);
    }
    else
    {
        *dir = m3dVUnitZ;
    }
}
