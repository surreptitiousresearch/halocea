#pragma once
#include "hkVector4.h"
#include "hkArray.h"
#include "hkBool.h"
#include "hkGeomHull.h"

/* hkGeomConvexHullMode — DEVIATION: FAST was 0 and ACCURATE was an abbreviated
   spelling; DB types_enum_values carries all four enumerators verbatim
   (INVALID=0, FAST=1, ACCURATE_BUT_SLOW=2, MAX_ID=3) and the binary agrees
   (findConnectivity @0x83F937FC passes li r7,1; generateConvexHull @0x83FDEB8C
   tests mode-2 for m_accurateButSlow). */
typedef enum hkGeomConvexHullMode
{
    HK_GEOM_CONVEXHULL_MODE_INVALID           = 0,
    HK_GEOM_CONVEXHULL_MODE_FAST              = 1,
    HK_GEOM_CONVEXHULL_MODE_ACCURATE_BUT_SLOW = 2,
    HK_GEOM_CONVEXHULL_MODE_MAX_ID            = 3
} hkGeomConvexHullMode;

/* hkGeomConvexHullTolerances (size 52, DB types_members). */
typedef struct hkGeomConvexHullTolerances
{
    hkBool m_postFilter;                  /* 0  */
    hkBool m_runConvertToUnitCube;        /* 1  */
    hkBool m_accurateButSlow;             /* 2  */
    float  m_weld_tolerance;              /* 4  */
    float  m_degenerate_tolerance;        /* 8  */
    float  m_coplanar_vertices_tolerance; /* 12 */
    float  m_coplanar_plane_tolerance;    /* 16 */
    float  m_coplanar_tolerance;          /* 20 */
    float  m_oppositeNormal_tolerance;    /* 24 */
    float  m__min_proj;                   /* 28 */
    float  m__maxAngle;                   /* 32 */
    float  m__planeEqnMinLength;          /* 36 */
    float  m__tol;                        /* 40 */
    float  m__tol2;                       /* 44 */
    float  m__tol3;                       /* 48 */
} hkGeomConvexHullTolerances;

/* hkGeomConvexHullBuilder::PlaneAndPoints (size 32, DB types_members) — a candidate
   face plane together with the three hull half-edges (m_v0/m_v1/m_v2) whose vertices
   define it. m_info is a packed status word. */
typedef struct hkGeomConvexHullBuilder_PlaneAndPoints
{
    hkVector4  m_planeEquation;    /* 0  */
    hkGeomEdge *m_v0;              /* 16 */
    hkGeomEdge *m_v1;              /* 20 */
    hkGeomEdge *m_v2;              /* 24 */
    unsigned short m_info;         /* 28 */
} hkGeomConvexHullBuilder_PlaneAndPoints;

void hkGeomConvexHullBuilder_generateConvexHull(const hkVector4 *verts, int numVertices,
                                                hkGeomHull *hullOut, hkArray<hkVector4> *usedVerticesOut,
                                                hkGeomConvexHullMode mode); /* hkGeomConvexHullBuilder::generateConvexHull */
