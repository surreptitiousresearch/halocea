#include <math.h>
#include "m3dBOX.h"
#include "m3d_boundary.h"

// m3dBOX::Calc @ 0x8265FD80
// Fit the axis-aligned box to `nPoint` points: seed both corners from the first point, then
// expand to include every remaining point. Finally enforce a minimum side length: any axis
// whose span is below `sideMinLen` is widened to exactly `sideMinLen` (max corner pushed out).
// The STRONG_ASSERT2_HELPER::asserd<m3dV> calls are the shipped finiteness assertions.
// DEVIATION: decompiler typed `sideMinLen` as double; mangle ?...@@QAAXPAUm3dV@@HM@Z (M) is float.
void m3dBOX::Calc(m3dV *pointList, int nPoint, float sideMinLen)
{
    fur.x = pointList->x;
    bll.x = pointList->x;
    bll.y = pointList->y;
    bll.z = pointList->z;
    fur.z = pointList->z;
    fur.y = pointList->y;

    if (nPoint > 1)
    {
        m3dV *point = pointList + 1;
        int remaining = nPoint - 1;
        do
        {
            if (!IGNORE_STRONG_ASSERT)
            {
                if (!m3dCheckFinite(point->x))
                    STRONG_ASSERT2_HELPER::asserd<m3dV>(
                        "m3dCheckFinite(pointList->x)",
                        "D:\\Projects\\code\\common\\src.sys\\m3d\\m3d_box.cpp", 666,
                        "*pointList", *point);
                if (!IGNORE_STRONG_ASSERT)
                {
                    if (!m3dCheckFinite(point->y))
                        STRONG_ASSERT2_HELPER::asserd<m3dV>(
                            "m3dCheckFinite(pointList->y)",
                            "D:\\Projects\\code\\common\\src.sys\\m3d\\m3d_box.cpp", 667,
                            "*pointList", *point);
                    if (!IGNORE_STRONG_ASSERT && !m3dCheckFinite(point->z))
                        STRONG_ASSERT2_HELPER::asserd<m3dV>(
                            "m3dCheckFinite(pointList->z)",
                            "D:\\Projects\\code\\common\\src.sys\\m3d\\m3d_box.cpp", 668,
                            "*pointList", *point);
                }
            }

            float px = point->x;
            if (fur.x < px) fur.x = point->x;
            float py = point->y;
            if (fur.y < py) fur.y = point->y;
            float pz = point->z;
            if (fur.z < pz) fur.z = point->z;
            if (bll.x > px) bll.x = px;
            if (bll.y > py) bll.y = py;
            if (bll.z > pz) bll.z = pz;

            --remaining;
            ++point;
        }
        while (remaining);
    }

    if (fabsf(fur.x - bll.x) < sideMinLen) fur.x = bll.x + sideMinLen;
    if (fabsf(fur.y - bll.y) < sideMinLen) fur.y = bll.y + sideMinLen;
    if (fabsf(fur.z - bll.z) < sideMinLen) fur.z = bll.z + sideMinLen;
}
