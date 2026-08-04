#include "../headers/havok/hkpConvexVerticesShape.h"
#include "../headers/havok/hkStridedVertices.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkpShapeType.h"

/* hkpConvexVerticesShape::`vftable' — installed as the object's virtual table. */
extern void *hkpConvexVerticesShape_vftable[]; /* hkpConvexVerticesShape::`vftable' */

/* hkpConvexVerticesShape::hkpConvexVerticesShape(hkStridedVertices, planeEquations,
   radius) — build a convex-hull shape from a strided vertex view and a set of
   plane equations. Initialise the shape base (type HK_SHAPE_CONVEX_VERTICES = 6,
   radius, refcount 1), start both geometry arrays empty, copy the caller's plane
   equations into m_planeEquations (growing it if needed), then pack the vertices
   into the rotated-vertex block via copyVertexData.

   DEVIATION: the per-element plane-equation copy is a VMX128 lvx128/stvx128 loop
   over 16-byte hkVector4s in the DB; reproduced here as a scalar element copy. */
void hkpConvexVerticesShape_construct(hkpConvexVerticesShape *self,
                                      const hkStridedVertices *verts,
                                      const hkArray<hkVector4> *planeEquations,
                                      float radius)
{
    hkThreadMemory *tm = hkThreadMemory_getCurrent();
    int wantCount = planeEquations->m_size;
    int capAndFlags;
    int i;

    self->m_radius = radius;
    self->m_userData = 0;
    self->base.__vftable = (hkBaseObject_vtbl *)hkpConvexVerticesShape_vftable;
    self->base.m_referenceCount = 1;
    self->m_type.m_storage = HK_SHAPE_CONVEX_VERTICES;

    self->m_rotatedVertices.m_capacityAndFlags = (int)0x80000000;
    self->m_rotatedVertices.m_data = 0;
    self->m_rotatedVertices.m_size = 0;

    self->m_planeEquations.m_capacityAndFlags = (int)0x80000000;
    self->m_planeEquations.m_data = 0;
    self->m_planeEquations.m_size = 0;
    self->m_connectivity = 0;

    capAndFlags = self->m_planeEquations.m_capacityAndFlags;
    if ((capAndFlags & 0x3FFFFFFF) < wantCount)
    {
        if ((capAndFlags & 0x80000000) == 0)
            hkThreadMemory_deallocateChunk(tm, self->m_planeEquations.m_data,
                                           16 * capAndFlags, HK_MEMORY_CLASS_ARRAY);
        self->m_planeEquations.m_data =
            (hkVector4 *)hkThreadMemory_allocateChunk(tm, 16 * wantCount, HK_MEMORY_CLASS_ARRAY);
        self->m_planeEquations.m_capacityAndFlags =
            (self->m_planeEquations.m_capacityAndFlags & 0x40000000) | wantCount;
    }
    self->m_planeEquations.m_size = wantCount;
    for (i = 0; i < wantCount; ++i)
        self->m_planeEquations.m_data[i] = planeEquations->m_data[i];

    hkpConvexVerticesShape_copyVertexData(self, verts->m_vertices, verts->m_striding,
                                          verts->m_numVertices);
}
