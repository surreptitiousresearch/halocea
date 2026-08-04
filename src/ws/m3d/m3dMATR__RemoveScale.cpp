#include "m3dMATR.h"

// m3dMATR::RemoveScale @ 0x825449B8
// Normalise the three basis axes (rows v[0..2]) to unit length by multiplying each by
// its inverse scale, leaving the origin (v[3]) untouched.
void m3dMATR::RemoveScale()
{
    float invScaleX, invScaleY, invScaleZ;
    GetInvScale(&invScaleX, &invScaleY, &invScaleZ);

    v[0].x *= invScaleX;
    v[0].y *= invScaleX;
    v[0].z *= invScaleX;

    v[1].x *= invScaleY;
    v[1].y *= invScaleY;
    v[1].z *= invScaleY;

    v[2].x *= invScaleZ;
    v[2].y *= invScaleZ;
    v[2].z *= invScaleZ;
}
