#include "m3dBOX.h"
#include "m3dV.h"

// m3dBOX::MakePointPoint @ 0x8265EC58
// Build the box spanning two arbitrary points: bll = componentwise min, fur = componentwise max.
void m3dBOX::MakePointPoint(m3dV *p1, m3dV *p2)
{
    m3dV minCorner;
    m3dV maxCorner;

    minCorner.x = (p1->x >= p2->x) ? p2->x : p1->x;
    minCorner.y = (p1->y >= p2->y) ? p2->y : p1->y;
    minCorner.z = (p1->z >= p2->z) ? p2->z : p1->z;

    maxCorner.x = (p1->x <= p2->x) ? p2->x : p1->x;
    maxCorner.y = (p1->y <= p2->y) ? p2->y : p1->y;
    maxCorner.z = (p1->z <= p2->z) ? p2->z : p1->z;

    bll = minCorner;
    fur = maxCorner;
}
