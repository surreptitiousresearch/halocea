#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWAH_SYS.h"        // wah.arrPlayers / PeekPlayer
#include "../../headers/ws/ai/aiWATCHER.h"         // aiWATCHER::GetBodyPos
#include "../../headers/ws/ai/aiCON_CB_globals.h"  // global aiPlanner
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"     // _m3dCheckValid

// aiBRAIN::GetDistToClosestPlayer() @ 0x832AD958
// ?GetDistToClosestPlayer@aiBRAIN@@QBAMXZ
//
// Distance from this brain's body to the nearest active player watcher (a large sentinel when
// there are no players). Scans the planner's player-watcher set.
float aiBRAIN::GetDistToClosestPlayer() const
{
    m3dV myPos = GetPos();

    float best = 2000000.0f;
    aiPLANNER *planner = aiPlanner;
    for (int i = 0; i < planner->wah.arrPlayers.nElem; ++i)
    {
        aiWATCHER *watcher = planner->wah.PeekPlayer(i);
        if (watcher)
        {
            m3dV playerPos;
            watcher->GetBodyPos(&playerPos);
            _m3dCheckValid(&myPos);
            _m3dCheckValid(&playerPos);

            float dx = myPos.x - playerPos.x;
            _m3dCheckValid(dx);
            float dy = myPos.y - playerPos.y;
            _m3dCheckValid(dy);
            float dz = myPos.z - playerPos.z;
            float distSq = dx * dx + dy * dy + dz * dz;
            _m3dCheckValid(dz);
            _m3dCheckValid(distSq);

            float dist = __fsqrts(distSq);
            if (dist < best)
                best = dist;
        }
        planner = aiPlanner;
    }
    return best;
}
