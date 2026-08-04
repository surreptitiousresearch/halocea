// aiCHASER::CalcRecentDeathPenalty @ 0x8320B590
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// Penalty for `pos` scaled by how recently/closely a friendly died: linearly maps distance
// [0..3] from the recorded death position to [1000..0], or 0 when the death timer has expired.
float aiCHASER::CalcRecentDeathPenalty(const m3dV *pos)
{
    if (this->timerRecentDeath >= 0.000001f)
    {
        float dist = m3dDist(pos, &this->posRecentDeath);
        return m3dClamp(0.0f, 1000.0f, 3.0f, 0.0f, dist);
    }
    return 0.0f;
}
