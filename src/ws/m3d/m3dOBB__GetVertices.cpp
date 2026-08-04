#include "m3dOBB.h"
#include "m3d_boundary.h"

// m3dOBB::GetVertices @ 0x82A6EE58
// Emit the 8 corners of the oriented box into vList[0..7]. The base quad is
// {org, org+X, org+X+Y, org+Y}; the top quad is the base quad translated by the Z extent.
// (X = vx*sx, Y = vy*sy, Z = vz*sz.) _m3dCheckValid calls are the shipped debug validity
// assertions, preserved.
void m3dOBB::GetVertices(m3dV *vList) const
{
    vList[0] = org;

    _m3dCheckValid(&org);
    _m3dCheckValid(&vx);
    _m3dCheckValid(sx);
    vList[1].x = vx.x * sx + org.x;
    vList[1].y = vx.y * sx + org.y;
    vList[1].z = vx.z * sx + org.z;

    _m3dCheckValid(&vList[1]);
    _m3dCheckValid(&vy);
    _m3dCheckValid(sy);
    vList[2].x = vy.x * sy + vList[1].x;
    vList[2].y = vy.y * sy + vList[1].y;
    vList[2].z = vy.z * sy + vList[1].z;

    _m3dCheckValid(&vList[0]);
    _m3dCheckValid(&vy);
    _m3dCheckValid(sy);
    vList[3].x = vy.x * sy + vList[0].x;
    vList[3].y = vy.y * sy + vList[0].y;
    vList[3].z = vy.z * sy + vList[0].z;

    _m3dCheckValid(&vList[0]);
    _m3dCheckValid(&vz);
    _m3dCheckValid(sz);
    vList[4].x = vz.x * sz + vList[0].x;
    vList[4].y = sz * vz.y + vList[0].y;
    vList[4].z = vz.z * sz + vList[0].z;

    _m3dCheckValid(&vList[1]);
    _m3dCheckValid(&vz);
    _m3dCheckValid(sz);
    vList[5].x = vz.x * sz + vList[1].x;
    vList[5].y = sz * vz.y + vList[1].y;
    vList[5].z = vz.z * sz + vList[1].z;

    _m3dCheckValid(&vList[2]);
    _m3dCheckValid(&vz);
    _m3dCheckValid(sz);
    vList[6].x = vz.x * sz + vList[2].x;
    vList[6].y = sz * vz.y + vList[2].y;
    vList[6].z = vz.z * sz + vList[2].z;

    _m3dCheckValid(&vList[3]);
    _m3dCheckValid(&vz);
    _m3dCheckValid(sz);
    vList[7].x = vz.x * sz + vList[3].x;
    vList[7].y = sz * vz.y + vList[3].y;
    vList[7].z = vz.z * sz + vList[3].z;
}
