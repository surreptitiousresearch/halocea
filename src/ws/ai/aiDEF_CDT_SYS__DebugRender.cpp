// aiDEF_CDT_SYS::DebugRender @ 0x83246618
// ?DebugRender@aiDEF_CDT_SYS@@QAAXXZ
#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/wb/dbgVAR_boundary.h" // dbgVAR_IMPL<bool,1> (.value)
#include "../../headers/ws/m3d/m3dV.h"

// ?rendDrv@@3PAVrendDRIVER@@A — the debug render driver. boundary data.
extern rendDRIVER *rendDrv;
// find-enemy debug-render toggle (dbgVAR con-var, `.value`), matching the aiDEBUG con-var
// convention. boundary data.
extern dbgVAR_IMPL<bool, 1> dbg_renderFindEnemy;
// active debug line/cross tint. boundary data.
extern unsigned int color;

// When the find-enemy debug toggle is on, draw each recorded shoot ray as a line from->to plus a
// cross at its endpoint.
// DEVIATION: the RenderCross colour register was lost by the decompiler (uninitialised r5); use the
// active debug tint `color`, matching the established aiDEBUG render convention.
void aiDEF_CDT_SYS::DebugRender()
{
    if (dbg_renderFindEnemy.value && dbgRays.nEntry > 0)
    {
        for (int i = 0; i < dbgRays.nEntry; ++i)
        {
            const FROM_TO &ray = dbgRays.list[i];
            rendDrv->RenderLine(&ray.from, &ray.to, color);
            rendDrv->RenderCross(&ray.to, 0.2f, color);
        }
    }
}
