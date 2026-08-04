// ws-engine ai08 (a8_ext_iface.cpp): aiBRAIN_IFACE::LocalHeightAndDistXZ @0x83193800.
// Virtual const. Decompose the from->to world offset into its vertical (up) and horizontal
// (flat-XZ) components via the DecompToUpAndFlatXZ virtual, and report the length of each.
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/m3d/m3d_boundary.h"   // _m3dCheckValid / m3dLengthVector

void aiBRAIN_IFACE::LocalHeightAndDistXZ(const m3dV &from, const m3dV &to,
                                         float &heightDiff, float &distXZ) const
{
    _m3dCheckValid(&to);
    _m3dCheckValid(&from);

    m3dV delta;
    delta.x = to.x - from.x;
    delta.y = to.y - from.y;
    delta.z = to.z - from.z;

    m3dV up;
    m3dV flatXZ;
    DecompToUpAndFlatXZ(delta, up, flatXZ);

    heightDiff = m3dLengthVector(&up);
    distXZ     = m3dLengthVector(&flatXZ);
}
