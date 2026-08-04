#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkMatrix4.h"
#include "../headers/havok/hkThreadMemory.h"

extern hkBaseObject_vtbl hkdGeometry_vftable; /* hkdGeometry::`vftable' */
extern const int HK_MEMORY_CLASS_DESTRUCTION;
extern void hkMatrix4_set(hkMatrix4 *out, const hkTransform *t);                                   /* hkMatrix4::set */
extern void hkdGeometry_set(hkdGeometry *out, const hkdGeometry *src, const hkMatrix4 *transform); /* hkdGeometry::set */
extern void hkdGeometry_concat(hkdGeometry *dst, const hkdGeometry *src);                          /* hkdGeometry::concat */
extern void hkdGeometry_destruct(hkdGeometry *self);                                               /* hkdGeometry::~hkdGeometry */

/* Reproduces the inlined hkdGeometry default constructor (empty arrays,
   self-parent). m_memSizeAndFlags is the allocation header and is set only on
   the heap instance, not the stack temp. */
static void geometry_initEmpty(hkdGeometry *g)
{
    g->base.m_referenceCount = 1;
    g->base.__vftable = &hkdGeometry_vftable;
    g->m_faces.m_data = 0;     g->m_faces.m_size = 0;     g->m_faces.m_capacityAndFlags = 0x80000000;
    g->m_triangles.m_data = 0; g->m_triangles.m_size = 0; g->m_triangles.m_capacityAndFlags = 0x80000000;
    g->m_vertices.m_data = 0;  g->m_vertices.m_size = 0;  g->m_vertices.m_capacityAndFlags = 0x80000000;
    g->m_objectIds.m_data = 0; g->m_objectIds.m_size = 0; g->m_objectIds.m_capacityAndFlags = 0x80000000;
    g->m_parent = g;
}

/* Build a single geometry that is the union of every child shape's geometry,
   each transformed into the parent frame. */
const hkdGeometry *SphereGun_createCompoundGeometry(const hkdBreakableShape *shape)
{
    hkdShapeInstanceInfo *children = (hkdShapeInstanceInfo *)shape->m_children.m_data;
    int count = shape->m_children.m_size;
    hkdGeometry *result;
    hkdGeometry temp;
    hkMatrix4 childMatrix;
    int i;

    result = (hkdGeometry *)hkThreadMemory_allocateChunk(hkThreadMemory_getCurrent(), 60, HK_MEMORY_CLASS_DESTRUCTION);
    geometry_initEmpty(result);
    result->base.m_memSizeAndFlags = 60;

    geometry_initEmpty(&temp);

    for (i = 0; i < count; ++i)
    {
        const hkdGeometry *childGeom = children[i].m_shape->m_geometry;
        if (childGeom)
        {
            hkMatrix4_set(&childMatrix, &children[i].m_transform);
            hkdGeometry_set(&temp, childGeom, &childMatrix);
            hkdGeometry_concat(result, &temp);
        }
    }

    hkdGeometry_destruct(&temp);
    return result;
}
