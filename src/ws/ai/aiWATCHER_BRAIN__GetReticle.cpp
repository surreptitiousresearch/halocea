#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // m3dVZero / m3dVUnitZ constants
#include "../../headers/ws/m3d/m3dV.h"

// aiWATCHER_BRAIN::GetReticle(m3dV&) const @ 0x8316E8E0  (virtual)
// Return the cached reticle direction and write the cached reticle start position into `eye`, but
// only while the watcher is valid; otherwise return the world +Z axis and a zero start point.
// NOTE: the decompiler mislabels the sret return slot as `this` and `this` as `result`; the ABI
// is m3dV(sret) GetReticle(this, m3dV& eye). The validity gate is the aiWATCHER vtbl+0x8 IsValid.
m3dV aiWATCHER_BRAIN::GetReticle(m3dV &eye) const
{
    if (const_cast<aiWATCHER_BRAIN *>(this)->IsValid())
    {
        eye = this->cachedReticleStart;
        return this->cachedReticleDir;
    }

    eye = m3dVZero;
    return m3dVUnitZ;
}
