// scrSCORCH_INI::scrSCORCH_INI @0x827454A8 — default-construct a scorch/decal descriptor:
// stamp the msgDATA header (sign=64), adopt the shared empty-string singleton for nameClass,
// zero-init cdtInfo, default the scales/radius to 1.0 and the rest to 0, then set sizeStruct=84.
// See src/headers/ws/scorch/scrSCORCH_INI.h for the DB-verified layout.
#include "../../headers/ws/scorch/scrSCORCH_INI.h"

extern const m3dV m3dVZero;

scrSCORCH_INI::scrSCORCH_INI()
{
    base.sign = 64;
    base.sizeStruct = -1;               // provisional; finalized to 84 at the end (matches disasm)

    nameClass.pBuffer = nullptr;
    nameClass.UnsafeInitEmpty();        // adopt shared empty-string buffer (ref-bump inlined)

    cdtInfo = cdtINFO();                // zero-init the 36-byte collision record

    texIdx = 0;
    pAtkDisp = nullptr;
    alphaScale = 1.0f;
    radScale = 1.0f;
    radius = 1.0f;
    rotAngle = 0.0f;
    normal = m3dVZero;

    base.sizeStruct = 84;
}
