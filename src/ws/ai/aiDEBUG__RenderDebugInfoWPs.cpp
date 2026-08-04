// ws-engine ai08 (a8_debug.cpp): aiDEBUG::RenderDebugInfoWPs @0x83193D88 (static).
// For each waypoint id in debugInfo[0..count): draw a debug cross at its effective position, and
// (for leaf waypoints on a high-obstacle edge) a green line along the HO-plane normal.
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"   // rendDrv / wpWP
#include "../../headers/ws/ai/aiPLANNER.h"            // aiPLANNER::GetWP
#include "../../headers/ws/ai/aiCON_CB_globals.h"     // aiPlanner
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/rend/rendDRIVER.h"

void aiDEBUG::RenderDebugInfoWPs(const short *debugInfo, int count, unsigned long color)
{
    for (int i = 0; i < count; ++i) {
        navWP *wp = aiPlanner->GetWP(static_cast<unsigned short>(debugInfo[i]));
        if (!wp)
            continue;

        m3dV pos;
        wp->GetEffectivePos(&pos);
        rendDrv->RenderCross(&pos, 0.2f, color);

        if (!wp->wpData->HaveChilds() && wp->IsHO()) {
            const m3dV *dir = wp->GetDirToHOPlane();
            m3dV end;
            end.x = dir->x;
            end.y = dir->y;
            end.z = dir->z;
            end += pos;
            rendDrv->RenderLine(&pos, &end, 0xFF00FF00);
        }
    }
}
