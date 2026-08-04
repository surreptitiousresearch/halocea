#pragma once
#include "hkVector4.h"

/* hkGeometryMatchingUtils::Geometry — a lightweight triangle-soup descriptor
   consumed by hkGeometryMatchingUtils::matchTriangles: a vertex array and a flat
   3-per-triangle index array. size 16. (The DB spells the second field's exact
   name opaquely; it is a per-geometry tag left -1 for the reference set.) */
typedef struct hkGeometryMatchingUtils_Geometry
{
    const hkVector4 *m_vertices;         /* 0  */
    int m_tag;                           /* 4  (reference set uses -1) */
    const int *m_triangleIndices;        /* 8  */
    int m_numTriangles;                  /* 12 */
} hkGeometryMatchingUtils_Geometry;
