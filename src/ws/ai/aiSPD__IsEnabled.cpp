#include "../../headers/ws/ai/aiGOAL_BASE.h"

// aiSPD::IsEnabled @ 0x83186130
// ?IsEnabled@aiSPD@@QBA_NXZ
//
// A gait is enabled unless it is flagged forced-only, and its effective max distance is positive.
// The effective distance is tempMaxDist, or the base maxDist when tempMaxDist is negative (unset).
bool aiSPD::IsEnabled() const
{
    if (this->isForcedOnly)
        return false;

    float dist = this->tempMaxDist;
    if (dist < 0.0f)
        dist = this->maxDist;

    return dist > 0.0f;
}
