#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiBRAIN_IFACE::DecompToUpAndFlatXZ(const m3dV&, m3dV&, m3dV&) const @ 0x831936F8
// Decompose `src` relative to the actor's up-direction: `up` is the projection of `src` onto the
// (unit) up-direction and `flatXZ` is the remaining (in-plane) component.
void aiBRAIN_IFACE::DecompToUpAndFlatXZ(const m3dV &src, m3dV &up, m3dV &flatXZ) const
{
    m3dV dirUp = GetDirUp();

    float dot = (src.z * dirUp.z) + ((src.x * dirUp.x) + (src.y * dirUp.y));
    _m3dCheckValid(dot);
    _m3dCheckValid(&dirUp);

    up.x = dirUp.x * dot;
    up.y = dirUp.y * dot;
    up.z = dirUp.z * dot;

    _m3dCheckValid(&src);
    _m3dCheckValid(&up);

    flatXZ.x = src.x - up.x;
    flatXZ.y = src.y - up.y;
    flatXZ.z = src.z - up.z;
}
