#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiDEBUG.h"

// aiVISION_COMMON::ProcessFrame @ 0x83292718
// ?ProcessFrame@aiVISION_COMMON@@UAAXXZ
//
// Per-frame vision update: recompute the effective sight direction (mirrored into the debug block),
// refresh the view-percent, and export the result.
void aiVISION_COMMON::ProcessFrame()
{
    m3dV dir;
    CalculateSightDir(&dir);
    dirSight            = dir;
    pBrain->dbg.dirSight = dir;

    UpdateViewPercent();
    ExportAll();
}
