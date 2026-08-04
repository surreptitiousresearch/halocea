#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // canonical m3dVZero

// aiWATCHER::GetFaceDir @ 0x8328D9A0
// ?GetFaceDir@aiWATCHER@@UBA?AUm3dV@@XZ
// Virtual const: return the direction component produced by the (virtual) GetFaceParams; the
// position component is written to a scratch vector and discarded. Both out vectors are pre-seeded
// with m3dVZero before the call.
m3dV aiWATCHER::GetFaceDir() const
{
    // GetFaceParams is declared non-const on the base; the binary calls it from this const method.
    aiWATCHER *self = const_cast<aiWATCHER *>(this);

    m3dV facePos = m3dVZero;
    m3dV faceDir = m3dVZero;
    self->GetFaceParams(&facePos, &faceDir);
    return faceDir;
}
