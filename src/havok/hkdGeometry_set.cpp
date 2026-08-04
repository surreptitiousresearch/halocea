#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkThreadMemory.h"
#include <string.h>

/* Reallocate an hkArray's buffer to hold at least newSize elements of elemSize
   bytes when its current capacity is too small, preserving the LOCKED flag and
   freeing any previously heap-owned buffer. Mirrors the inlined grow the
   compiler emitted for each array below. Returns the (possibly new) data ptr. */
static void *hkd_grow(void **data, int *capAndFlags, int newSize, int elemSize)
{
    int cap = *capAndFlags;
    if ((cap & 0x3FFFFFFF) < newSize)
    {
        hkThreadMemory *mem = hkThreadMemory_getCurrent();
        void *chunk;
        if ((cap & 0x80000000) == 0)
            hkThreadMemory_deallocateChunk(mem, *data, elemSize * cap, HK_MEMORY_CLASS_ARRAY);
        chunk = hkThreadMemory_allocateChunk(mem, elemSize * newSize, HK_MEMORY_CLASS_ARRAY);
        *data = chunk;
        *capAndFlags = (*capAndFlags & 0x40000000) | newSize;
    }
    return *data;
}

/* hkdGeometry::set — deep-copy every array of `rhs` into `this` (faces,
   triangles, vertices, object identifiers), then mark this geometry as its own
   parent. DEVIATION: the original copies each element with unrolled 16-bit word
   loops / VMX128 quadword moves and copies ObjectIdentifier field-by-field;
   reconstructed as capacity-checked reallocs plus a memcpy of the whole element
   run (byte-identical result). Element sizes: Face 16, Triangle 12,
   hkVector4 16, ObjectIdentifier 80. */
void hkdGeometry_set(hkdGeometry *self, const hkdGeometry *rhs)
{
    hkd_grow((void **)&self->m_faces.m_data, &self->m_faces.m_capacityAndFlags,
             rhs->m_faces.m_size, 16);
    self->m_faces.m_size = rhs->m_faces.m_size;
    if (rhs->m_faces.m_size > 0)
        memcpy(self->m_faces.m_data, rhs->m_faces.m_data, (size_t)rhs->m_faces.m_size * 16);

    hkd_grow((void **)&self->m_triangles.m_data, &self->m_triangles.m_capacityAndFlags,
             rhs->m_triangles.m_size, 12);
    self->m_triangles.m_size = rhs->m_triangles.m_size;
    if (rhs->m_triangles.m_size > 0)
        memcpy(self->m_triangles.m_data, rhs->m_triangles.m_data, (size_t)rhs->m_triangles.m_size * 12);

    hkd_grow((void **)&self->m_vertices.m_data, &self->m_vertices.m_capacityAndFlags,
             rhs->m_vertices.m_size, 16);
    self->m_vertices.m_size = rhs->m_vertices.m_size;
    if (rhs->m_vertices.m_size > 0)
        memcpy(self->m_vertices.m_data, rhs->m_vertices.m_data, (size_t)rhs->m_vertices.m_size * 16);

    hkd_grow((void **)&self->m_objectIds.m_data, &self->m_objectIds.m_capacityAndFlags,
             rhs->m_objectIds.m_size, 80);
    self->m_objectIds.m_size = rhs->m_objectIds.m_size;
    if (rhs->m_objectIds.m_size > 0)
        memcpy(self->m_objectIds.m_data, rhs->m_objectIds.m_data, (size_t)rhs->m_objectIds.m_size * 80);

    self->m_parent = self;
}
