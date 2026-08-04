#include <math.h>
#include "m3dV.h"
#include "m3dMATR.h"
#include "m3d_boundary.h"

// m3dMATR::_MakeLCS2WCS_VZ @ 0x82543C00
// Build a local-to-world transform from an origin and a forward direction that becomes the
// Z axis. The up axis is derived from world Y (or world X if `dir` is nearly parallel to Y),
// made orthogonal to `dir` (Gram-Schmidt), then MakeLCS2WCS fills in the remaining axis.
void m3dMATR::_MakeLCS2WCS_VZ(m3dV *org, m3dV *dir)
{
    m3dV reference;
    m3dV forward;
    m3dV up;
    m3dV projection;

    forward.x = dir->x;
    forward.y = dir->y;
    forward.z = dir->z;
    m3dNormalize(&forward);

    float dotWorldUp = m3dVUnitY.x * forward.x + m3dVUnitY.y * forward.y + m3dVUnitY.z * forward.z;
    if (fabsf(dotWorldUp) <= 0.5f)
        reference = m3dVUnitY;
    else
        reference = m3dVUnitX;

    // Remove the component of `reference` along `forward`, leaving an orthogonal up axis.
    m3dNormalComponent(&reference, &forward, &projection);
    up.x = reference.x - projection.x;
    up.y = reference.y - projection.y;
    up.z = reference.z - projection.z;
    m3dNormalize(&up);

    // vx null -> derived from up x forward; vy = up; vz = forward.
    MakeLCS2WCS(org, nullptr, &up, &forward);
}
