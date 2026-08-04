#include <math.h>
#include "m3dOBB.h"
#include "m3d_boundary.h"

// m3dOBB::GetSize @ 0x825E0DB0
// Length of the extents vector: sqrt(sx^2 + sy^2 + sz^2).
float m3dOBB::GetSize()
{
    _m3dCheckValid(sx);
    _m3dCheckValid(sy);
    _m3dCheckValid(sz);

    float lengthSquared = sx * sx + sy * sy + sz * sz;
    _m3dCheckValid(lengthSquared);
    return sqrtf(lengthSquared);
}
