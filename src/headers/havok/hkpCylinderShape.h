#pragma once
#include "hkVector4.h"
#include "hkpConvexShape.h"

/* types_members hkpCylinderShape (size 96) — a capped cylinder collision shape.
   Derives from hkpConvexShape (which carries the convex m_radius at offset 16).
   The two endpoint centres m_vertexA/m_vertexB sit at offsets 32/48 as
   consecutive hkVector4s, so getVertex(i) indexes them as an array. */
struct hkpCylinderShape : hkpConvexShape
{
    float m_cylRadius;                                    /* 20 */
    float m_cylBaseRadiusFactorForHeightFieldCollisions; /* 24 */
    /* 28: 4 bytes padding to the 16-byte alignment of m_vertexA */
    unsigned char _pad1C[4];                              /* 28 */
    hkVector4 m_vertexA;                                  /* 32 */
    hkVector4 m_vertexB;                                  /* 48 */
    hkVector4 m_perpendicular1;                           /* 64 */
    hkVector4 m_perpendicular2;                           /* 80 */
};
typedef struct hkpCylinderShape hkpCylinderShape;
