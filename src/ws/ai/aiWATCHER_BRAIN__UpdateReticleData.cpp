#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiWATCHER_BRAIN::UpdateReticleData() @ 0x8316E850
// Snapshot the tracked unit's current body position and facing direction into the cached reticle
// start/direction (both filled via the model-driven GetBodyPos / GetFaceDir virtuals).
void aiWATCHER_BRAIN::UpdateReticleData()
{
    m3dV bodyPos;
    this->cachedReticleStart = *this->GetBodyPos(&bodyPos);

    m3dV faceDir;
    this->cachedReticleDir = *this->GetFaceDir(&faceDir);
}
