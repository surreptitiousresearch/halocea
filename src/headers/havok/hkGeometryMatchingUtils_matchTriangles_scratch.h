#pragma once

/* Scratch record built by hkGeometryMatchingUtils::matchTriangles: a parallel
   table (one entry per flattened triangle) that maps a broadphase leaf key back
   to the geometry and local triangle it came from. 12-byte stride, matching the
   inlined stores in the decompiler (geometry index at +0, local index at +4). */
typedef struct hkMatchTriangleLeaf
{
    unsigned short m_geometryIndex;      /* 0 */
    unsigned short _pad0;                /* 2 */
    int m_localTriangleIndex;            /* 4 */
    int _pad8;                           /* 8 -> 12 */
} hkMatchTriangleLeaf;
