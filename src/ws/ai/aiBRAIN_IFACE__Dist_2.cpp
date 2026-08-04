#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiBRAIN_IFACE::Dist_2(const m3dV*) const @ 0x83193508
// Alternate distance metric from this actor's position to `posFrom`.
float aiBRAIN_IFACE::Dist_2(const m3dV *posFrom) const
{
    m3dV pos = GetPos();
    return m3dDist_2(&pos, posFrom);
}
