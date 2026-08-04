#include "../../headers/ws/ai/aiCMD2BODY.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// m3d subsystem — debug finiteness assert (side-effect only). boundary.
extern int _m3dCheckValid(const m3dV *v);

// aiCMD2BODY::LookStraight() @ 0x83175F68
// ?LookStraight@aiCMD2BODY@@QAAXXZ
// Aim the body's gaze straight ahead: a point 1.35 units up and 2.0 units forward from the
// brain's body position. Only latches when no explicit look-at was already requested this frame.
// DEVIATION: the original also asserts the scalar constants 2.0/1.35 via a scalar _m3dCheckValid
// overload; those debug-only scalar asserts are dropped (no observable effect).
void aiCMD2BODY::LookStraight()
{
    m3dV dir = pBrain->GetDir();
    _m3dCheckValid(&dir);
    m3dV forward;
    forward.x = dir.x * 2.0f;
    forward.y = dir.y * 2.0f;
    forward.z = dir.z * 2.0f;

    m3dV up = pBrain->GetDirUp();
    _m3dCheckValid(&up);
    m3dV upScaled;
    upScaled.x = up.x * 1.35f;
    upScaled.y = up.y * 1.35f;
    upScaled.z = up.z * 1.35f;

    m3dV pos = pBrain->GetPos();
    _m3dCheckValid(&pos);
    _m3dCheckValid(&upScaled);
    m3dV base;
    base.x = pos.x + upScaled.x;
    base.y = pos.y + upScaled.y;
    base.z = pos.z + upScaled.z;
    _m3dCheckValid(&base);
    _m3dCheckValid(&forward);

    if (!isSetPosLookAt)
    {
        isSetPosLookAt = true;
        posLookAt.x = base.x + forward.x;
        posLookAt.y = base.y + forward.y;
        posLookAt.z = base.z + forward.z;
    }
}
