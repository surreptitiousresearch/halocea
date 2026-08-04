// aiFILT_ENEMY::GetVisCheckParams @ 0x8321F3B0
#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/m3d/m3dV.h"

// Debug finiteness asserts (side-effect only). boundary.
void _m3dCheckValid(const m3dV *v);
void _m3dCheckValid(float value);

// Derive the visibility-check geometry for candidate `watcher` as seen from `brain`:
//   outEye    = watcher's body position (the "from" point of the LOS test)
//   outTarget = body-up * sampleDist + per-viewer adjustment (offset to the effective head)
// NOTE: the decompiler mistyped `watcher` as m3dV* and left the aiWATCHER virtuals as raw vtable
// offsets (0x34 GetBodyPos, 0x38 GetBodyDirUp, 0x60 GetVisCheckSampleDist) — resolved here.
void aiFILT_ENEMY::GetVisCheckParams(aiBRAIN *brain, aiWATCHER *watcher, m3dV *outEye, m3dV *outTarget)
{
    m3dV dirUp;
    watcher->GetBodyDirUp(&dirUp);

    float sampleDist = watcher->GetVisCheckSampleDist();

    m3dV bodyPos;
    *outEye = *watcher->GetBodyPos(&bodyPos);

    _m3dCheckValid(sampleDist);
    _m3dCheckValid(&dirUp);

    outTarget->x = dirUp.x * sampleDist;
    outTarget->y = dirUp.y * sampleDist;
    outTarget->z = dirUp.z * sampleDist;

    m3dV adjust = watcher->CalcAdjustVisChk(brain);
    *outTarget += adjust;
}
