#include "m3dV.h"
#include "m3dMATR.h"
#include "m3d_boundary.h"

// m3dMATR::MakeLCS2WCS @ 0x82541FD0
// Build a local-to-world transform from an origin and up to three basis axes. Any axis
// passed as null is derived from the cross product of the other two.
// NOTE: faithful to the original, a single scratch buffer backs every derived axis; if
// more than one axis is null the later cross products read and write that same buffer
// (the original's behaviour, preserved here).
void m3dMATR::MakeLCS2WCS(const m3dV *org, m3dV *vx, m3dV *vy, m3dV *vz)
{
    m3dV derivedAxis;
    m3dV *axisX = vx;
    m3dV *axisY = vy;
    m3dV *axisZ = vz;

    if (!axisX)
    {
        m3dCrossProduct(vy, vz, &derivedAxis);
        axisX = &derivedAxis;
    }
    if (!axisY)
    {
        m3dCrossProduct(axisZ, axisX, &derivedAxis);
        axisY = &derivedAxis;
    }
    if (!axisZ)
    {
        m3dCrossProduct(axisX, axisY, &derivedAxis);
        axisZ = &derivedAxis;
    }

    v[0].x = axisX->x; v[1].x = axisY->x; v[2].x = axisZ->x;
    v[0].y = axisX->y; v[1].y = axisY->y; v[2].y = axisZ->y;
    v[0].z = axisX->z; v[1].z = axisY->z; v[2].z = axisZ->z;

    v[3].x = org->x;
    v[3].y = org->y;
    v[3].z = org->z;

    v[0].w = 0.0f;
    v[1].w = 0.0f;
    v[2].w = 0.0f;
    v[3].w = 1.0f;
}
