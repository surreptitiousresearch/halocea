#pragma once
#include "hkArray.h"
#include "hkVector4.h"

/* hkGeometry::Triangle (size 12, DB types_members) — a triangle by vertex index.
   (DB also carries a material index in the full engine, but only a/b/c are
   referenced here.) */
typedef struct hkGeometry_Triangle
{
    int m_a; /* 0 */
    int m_b; /* 4 */
    int m_c; /* 8 */
} hkGeometry_Triangle;

/* hkGeometry (size 24, DB types_members) — indexed triangle mesh. */
typedef struct hkGeometry
{
    hkArray<hkVector4>           m_vertices;   /* 0  */
    hkArray<hkGeometry_Triangle> m_triangles;  /* 12 */
} hkGeometry;
