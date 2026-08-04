#pragma once
#include "hkVector4.h"
#include "hkInplaceArray.h"

/* hkGeomEdge (size 8, DB types_members) — one half-edge of a hull face loop. */
typedef struct hkGeomEdge
{
    unsigned short m_vertex; /* 0 */
    unsigned short m_mirror; /* 2 */
    unsigned short m_next;   /* 4 */
    unsigned short m_info;   /* 6 (bitfield :16) */
} hkGeomEdge;

/* hkGeomHull (size 1040, DB types_members) — convex-hull edge structure produced
   by hkGeomConvexHullBuilder. m_edges keeps up to 128 edges inline before it
   spills to the heap. */
typedef struct hkGeomHull
{
    hkVector4 *m_vertexBase;                    /* 0 */
    hkInplaceArray<hkGeomEdge, 128> m_edges;    /* 4 (1036 bytes) */
} hkGeomHull;

void hkGeomHull_construct(hkGeomHull *self); /* hkGeomHull::hkGeomHull */
