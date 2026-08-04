#include <math.h>
#include "m3dV.h"
#include "m3dMATR.h"
#include "m3d_boundary.h"

// m3dMATR::_MakeLCS2WCS_VY @ 0x82543D30
// Build a local-to-world transform from an origin and a direction that becomes the Y (up)
// axis. A forward axis is derived from world Y (or world X if `dir` is nearly parallel to Y),
// made orthogonal to `dir` (Gram-Schmidt), then MakeLCS2WCS fills in the remaining axis.
// Identical to _MakeLCS2WCS_VZ except the derived/passed axes map to vx/vy instead of vy/vz.
void m3dMATR::_MakeLCS2WCS_VY(m3dV *org, m3dV *dir)
{
    m3dV reference;
    m3dV upAxis;
    m3dV forward;
    m3dV projection;

    upAxis.x = dir->x;
    upAxis.y = dir->y;
    upAxis.z = dir->z;
    m3dNormalize(&upAxis);

    float dotWorldUp = m3dVUnitY.x * upAxis.x + m3dVUnitY.y * upAxis.y + m3dVUnitY.z * upAxis.z;
    if (fabsf(dotWorldUp) <= 0.5f)
        reference = m3dVUnitY;
    else
        reference = m3dVUnitX;

    // Remove the component of `reference` along `upAxis`, leaving an orthogonal forward axis.
    m3dNormalComponent(&reference, &upAxis, &projection);
    forward.x = reference.x - projection.x;
    forward.y = reference.y - projection.y;
    forward.z = reference.z - projection.z;
    m3dNormalize(&forward);

    // vx = forward; vy = upAxis; vz null -> derived from forward x upAxis.
    MakeLCS2WCS(org, &forward, &upAxis, nullptr);
}
