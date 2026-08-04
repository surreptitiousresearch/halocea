#include "../../headers/ws/ai/aiLOD_SYS.h"

extern int m3dUpdateTimeField(float *field, float dt); // count *field down by dt, clamp at 0

// aiLOD_SYS::ProcessFrame @ 0x83239EC8
// ?ProcessFrame@aiLOD_SYS@@QAAXM@Z
//
// Per-frame tick: age the LOD-recompute timer and do a full LOD recompute when it elapses, then
// age the strike load queue and advance the mind sparser.
void aiLOD_SYS::ProcessFrame(float delta)
{
    m3dUpdateTimeField(&timerUpdateLODs, delta);
    if (timerUpdateLODs < 0.000001f)
    {
        RecalculateLODs();
        timerUpdateLODs = 0.0f;
    }
    UpdateStrikePerfLoad(delta);
    UpdateMindSparser();
}
