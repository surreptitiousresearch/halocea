#include "m3dBOX.h"
#include "m3dMATR.h"
#include "m3dV.h"

// m3dBOX::Transform @ 0x8265FFF0  (overload __2 — higher address)
// Transform this box by a matrix and re-fit an axis-aligned box around the result:
// enumerate the 8 corners, transform them all, then compute their enclosing AABB.
void m3dBOX::Transform(m3dMATR *pLCS2WCS, m3dBOX *bb)
{
    m3dV corners[8];
    corners[0].x = bll.x; corners[0].y = bll.y; corners[0].z = bll.z;
    corners[1].x = fur.x; corners[1].y = bll.y; corners[1].z = bll.z;
    corners[2].x = fur.x; corners[2].y = fur.y; corners[2].z = bll.z;
    corners[3].x = bll.x; corners[3].y = fur.y; corners[3].z = bll.z;
    corners[4].x = bll.x; corners[4].y = bll.y; corners[4].z = fur.z;
    corners[5].x = fur.x; corners[5].y = bll.y; corners[5].z = fur.z;
    corners[6].x = fur.x; corners[6].y = fur.y; corners[6].z = fur.z;
    corners[7].x = bll.x; corners[7].y = fur.y; corners[7].z = fur.z;

    pLCS2WCS->TransformPointList(8, corners, sizeof(m3dV), corners, sizeof(m3dV));
    bb->Calc(corners, 8, 0.0f);
}
