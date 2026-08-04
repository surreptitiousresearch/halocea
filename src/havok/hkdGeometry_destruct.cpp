#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkThreadMemory.h"

/* boundary — the hkBaseObject vtable this destructor rebinds to on teardown. */
extern void *const hkBaseObject_vftable; /* hkBaseObject::`vftable' */

/* hkdGeometry::~hkdGeometry — free the four owned arrays (each released only
   when it owns heap storage, m_capacityAndFlags >= 0) then re-point the vtable
   at hkBaseObject. Element sizes: ObjectIdentifier 80, hkVector4 16,
   Triangle 12, Face 16. */
void hkdGeometry_destruct(hkdGeometry *self)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    int cap;

    cap = self->m_objectIds.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_objectIds.m_data,
                                       80 * (cap & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);

    cap = self->m_vertices.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_vertices.m_data,
                                       16 * cap, HK_MEMORY_CLASS_ARRAY);

    cap = self->m_triangles.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_triangles.m_data,
                                       12 * (cap & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);

    cap = self->m_faces.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_faces.m_data,
                                       16 * cap, HK_MEMORY_CLASS_ARRAY);

    self->base.__vftable = (hkBaseObject_vtbl *)&hkBaseObject_vftable;
}
