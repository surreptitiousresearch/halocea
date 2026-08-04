#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWAH_boundaries.h"

// aiWATCHER_BRAIN::GetPreferredEvadeDir @ 0x83170160
// ?GetPreferredEvadeDir@aiWATCHER_BRAIN@@UBA?AUm3dV@@ABU2@@Z
// Const virtual override returning m3dV by value. The reticle-space evade direction is read from
// the tracked brain's aiSTATUS block, normalised (falling back to a secondary status vector if
// degenerate), validated, then negated. `posEvader` is part of the interface but unused here.
// NOTE (ABI): the decompiler swaps `this` and the hidden m3dV return pointer for this signature;
// disassembly confirms r3 = return m3dV*, r4 = this (brain read at r4+0x4C = this->brain).
m3dV aiWATCHER_BRAIN::GetPreferredEvadeDir(const m3dV & /*posEvader*/) const
{
    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;

    // aiBRAIN::st.pointee — the runtime status block (raw offsets are aiSTATUS boundary fields).
    const char *st = reinterpret_cast<const char *>(pBrain->st.pointee);
    const float *evadeDir = reinterpret_cast<const float *>(st + aiSTATUS_off_evadeReticleDir);

    m3dV dir;
    dir.x = evadeDir[0];
    dir.y = evadeDir[1];
    dir.z = evadeDir[2];

    if (!_m3dNormalize(&dir))
    {
        const float *fallback = reinterpret_cast<const float *>(st + aiSTATUS_off_evadeFallback);
        dir.x = fallback[0];
        dir.y = fallback[1];
        dir.z = fallback[2];
    }
    _m3dCheckValid(&dir);

    m3dV result;
    result.x = -dir.x;
    result.y = -dir.y;
    result.z = -dir.z;
    return result;
}
