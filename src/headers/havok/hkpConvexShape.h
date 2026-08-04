#pragma once
#include "hkVector4.h"
#include "hkpShape.h"

/* types_members hkpSphereRepShape (size 16) — just the hkpShape base. */
struct hkpSphereRepShape : hkpShape
{
};
typedef struct hkpSphereRepShape hkpSphereRepShape;

/* types_members hkpConvexShape (size 20) — sphere-rep base + convex radius.
   Reached mostly through its virtual table. hkpCdVertex is a 16-byte padded
   position, layout-identical to hkVector4, used as the
   getSupportingVertexImpl output slot. */
struct hkpConvexShape : hkpSphereRepShape
{
    float m_radius; /* 16 */
};
typedef struct hkpConvexShape hkpConvexShape;
typedef hkVector4 hkpCdVertex;

/* getSupportingVertexImpl(direction, vertexOut) lives at vtable byte-offset 0x38
   (lwz r6,0x38(r7); mtctr; bctrl). */
typedef void (*hkpConvexShape_getSupportingVertexImpl_t)(const hkpConvexShape *shape,
                                                         const hkVector4 *direction,
                                                         hkpCdVertex *vertexOut);
