#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiBRAIN_IFACE::Dist(const m3dV*) const @ 0x831934B8
// 3-D Euclidean distance from this actor's position to `posFrom`.
float aiBRAIN_IFACE::Dist(const m3dV *posFrom) const
{
    m3dV pos = GetPos();
    return m3dDist(&pos, posFrom);
}
