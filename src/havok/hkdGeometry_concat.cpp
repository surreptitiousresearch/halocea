/* ?concat@hkdGeometry@@QAAXABV1@@Z @0x83851030 */
#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkArray.h"

/* boundary — object-id table lookup/insert and vertex splice (level-2 callees). */
extern int  hkdGeometry_findSourceObjectIdentifierIndex(hkdGeometry *self, const hkdGeometry_ObjectIdentifier *id); /* hkdGeometry::findSourceObjectIdentifierIndex */
extern int  hkdGeometry_addSourceObjectIdentifier(hkdGeometry *self, const hkdGeometry_ObjectIdentifier *id);       /* hkdGeometry::addSourceObjectIdentifier */
extern void hkArray_hkVector4_spliceInto(hkArray<hkVector4> *dst, int index, int numToRemove,
                                         const hkVector4 *src, int numToInsert);                        /* hkArray<hkVector4>::spliceInto */
extern void hkArrayUtil__reserveMore(void *array, int sizeElem);                               /* hkArrayUtil::_reserveMore */

#define HKD_INDEX_MAX 0x10000

/* hkdGeometry::concat — append geometry `geom` (referred through its shared
   parent) onto `this`, aborting (with a Havok warning in the original) if any of
   the vertex/face/triangle counts would overflow the 16-bit index space.
   Object identifiers are de-duplicated into this geometry via find/add and geom's
   local object indices are remapped through a scratch table; vertices are spliced
   in; triangles and faces are appended with their vertex/face/triangle indices
   rebased; finally every face's parentFaceIndex is reset to its own index.
   DEVIATION: the abort-path hkError warning messages are elided (debug only, the
   early return is preserved); the object-id remap table is allocated on the
   Havok per-thread scratch stack (the original inlines the bump/pop). */
void hkdGeometry_concat(hkdGeometry *self, const hkdGeometry *geom)
{
    hkThreadMemory *mem;
    hkdGeometry *geomParent;
    int *objIdRemap;
    int vertexBase, triangleBase, faceBase;
    int i;

    if (self->m_vertices.m_size + geom->m_vertices.m_size >= HKD_INDEX_MAX)
        return; /* too many vertices, aborting concat */
    if (geom->m_faces.m_size + self->m_faces.m_size >= HKD_INDEX_MAX)
        return; /* too many faces, aborting concat */
    if (geom->m_triangles.m_size + self->m_triangles.m_size >= HKD_INDEX_MAX)
        return; /* too many triangles, aborting concat */

    mem = hkThreadMemory_getCurrent();
    geomParent = geom->m_parent;

    /* Scratch table mapping geom's local object indices to this geometry's. */
    objIdRemap = (int *)hkThreadMemory_allocateStack(mem, (geomParent->m_objectIds.m_size + 4) * 4);
    for (i = 0; i < geomParent->m_objectIds.m_size; ++i)
    {
        const ObjectIdentifier *id = &geomParent->m_objectIds.m_data[i];
        int idx = hkdGeometry_findSourceObjectIdentifierIndex(self, id);
        if (idx < 0)
            idx = hkdGeometry_addSourceObjectIdentifier(self, id);
        objIdRemap[i] = idx;
        geomParent = geom->m_parent; /* reloaded each iteration as in the original */
    }

    vertexBase   = self->m_vertices.m_size;
    triangleBase = self->m_triangles.m_size;
    faceBase     = self->m_faces.m_size;

    hkArray_hkVector4_spliceInto(&self->m_parent->m_vertices, vertexBase, 0,
                                 geomParent->m_vertices.m_data, geom->m_vertices.m_size);

    for (i = 0; i < geom->m_triangles.m_size; ++i)
    {
        const Triangle *src = &geom->m_triangles.m_data[i];
        Triangle *dst;
        int j;
        if (self->m_triangles.m_size == (self->m_triangles.m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&self->m_triangles, 12);
        dst = &self->m_triangles.m_data[self->m_triangles.m_size++];
        for (j = 0; j < 3; ++j)
        {
            unsigned short nf = src->m_neighbouringFaces[j];
            dst->m_vertexIndices[j] = (unsigned short)(vertexBase + src->m_vertexIndices[j]);
            dst->m_neighbouringFaces[j] = (nf != 0xFFFF) ? (unsigned short)(nf + faceBase) : 0xFFFF;
        }
    }

    for (i = 0; i < geom->m_faces.m_size; ++i)
    {
        const Face *src = &geom->m_faces.m_data[i];
        Face *dst;
        if (self->m_faces.m_size == (self->m_faces.m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&self->m_faces, 16);
        dst = &self->m_faces.m_data[self->m_faces.m_size++];
        dst->m_startTriangleIndex = (unsigned short)(src->m_startTriangleIndex + triangleBase);
        dst->m_numTriangles = src->m_numTriangles;
        dst->m_objectIndex = (unsigned short)objIdRemap[src->m_objectIndex];
        dst->m_support = src->m_support;
    }

    for (i = 0; i < self->m_faces.m_size; ++i)
        self->m_faces.m_data[i].m_parentFaceIndex = (unsigned short)i;

    hkThreadMemory_deallocateStack(mem, objIdRemap);
}
