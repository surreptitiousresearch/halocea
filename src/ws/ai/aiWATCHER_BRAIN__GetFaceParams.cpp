#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::GetFaceParams @ 0x8316F448
// ?GetFaceParams@aiWATCHER_BRAIN@@UBAXAAUm3dV@@0@Z
// Virtual const: read the brain's face position & direction out of its status snapshot's body
// coordinate system (aiSTATUS::CS.posFace at float[36..38], .dirFace at float[39..41]). Falls back
// to (origin, +Z) when invalid.
void aiWATCHER_BRAIN::GetFaceParams(m3dV &pos, m3dV &dir) const
{
    if (!IsValid())
    {
        pos = m3dVZero;
        dir = m3dVUnitZ;
        return;
    }
    const aiSTATUS *status = brain.GetPtr()->st.pointee;
    pos = status->cs.posFace;
    dir = status->cs.dirFace;
}
