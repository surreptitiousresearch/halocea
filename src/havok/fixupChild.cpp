/* fixupChild @0x8383CAD0 */
/*
 * hkdBreakableShape asset-processing helper (destruction subsystem).
 *
 * fixupChild re-links a freshly cloned/loaded breakable shape's geometry back
 * to the "root" geometries it was authored against:
 *   1. For each object id of this shape's geometry, find the root geometry whose
 *      first object id carries the same name, building an index remap table.
 *   2. For each face, copy the FaceIdentifier user-data from the corresponding
 *      face of the matching root geometry (looked up through the remap table).
 *   3. Recurse into every child shape instance.
 *
 * Sibling of retransformResourceTree / getTransformOfNode.
 */

#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkdGeometry_Face.h"
#include "../headers/havok/hkdGeometry_ObjectIdentifier.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"

/* Havok SDK boundary — not reversed. */
extern int hkString_strCmp(const char *a, const char *b); /* hkString::strCmp */

void fixupChild(const hkdBreakableShape *shape, hkArray<> *rootGeometries)
{
    const hkdGeometry *geometry = shape->m_geometry.m_pntr;

    /* Local hkArray<int>: maps each object id of this geometry's parent to the
       matching root geometry index (or -1). Backed by inline stack storage;
       capacityAndFlags carries hkArray::DONT_DEALLOCATE_FLAG (0x80000000) so
       the inlined destructor at function exit is a statically-skipped no-op. */
    int objectIdRemapStorage[85];
    hkArray<> objectIdRemap;
    objectIdRemap.m_data = objectIdRemapStorage;
    objectIdRemap.m_capacityAndFlags = 0x80000040; /* capacity 64 | DONT_DEALLOCATE_FLAG */
    objectIdRemap.m_size = geometry->m_parent->m_objectIds.m_size;

    int objectIdCount = objectIdRemap.m_size;
    for (int objectIdIndex = 0; objectIdIndex < objectIdCount; ++objectIdIndex)
    {
        const hkdGeometry_ObjectIdentifier *objectIds =
            (const hkdGeometry_ObjectIdentifier *)geometry->m_parent->m_objectIds.m_data;
        const char *objectName = objectIds[objectIdIndex].m_name;

        ((int *)objectIdRemap.m_data)[objectIdIndex] = -1;

        for (int rootIndex = 0; rootIndex < rootGeometries->m_size; ++rootIndex)
        {
            const hkdGeometry *rootGeometry =
                ((const hkdGeometry **)rootGeometries->m_data)[rootIndex];
            const hkdGeometry_ObjectIdentifier *rootObjectIds =
                (const hkdGeometry_ObjectIdentifier *)rootGeometry->m_parent->m_objectIds.m_data;

            /* strCmp returns 0 on match. */
            if (!hkString_strCmp(objectName, rootObjectIds[0].m_name))
            {
                ((int *)objectIdRemap.m_data)[objectIdIndex] = rootIndex;
                break;
            }
        }
    }

    for (int faceIndex = 0; faceIndex < geometry->m_faces.m_size; ++faceIndex)
    {
        hkdGeometry_Face *face = &((hkdGeometry_Face *)geometry->m_faces.m_data)[faceIndex];

        int rootIndex = ((int *)objectIdRemap.m_data)[face->m_objectIndex];
        const hkdGeometry *rootGeometry =
            ((const hkdGeometry **)rootGeometries->m_data)[rootIndex];
        const hkdGeometry_Face *sourceFace =
            &((const hkdGeometry_Face *)rootGeometry->m_faces.m_data)[face->m_parentFaceIndex];

        face->m_support.m_userData[0] = sourceFace->m_support.m_userData[0];
        face->m_support.m_userData[1] = sourceFace->m_support.m_userData[1];
        face->m_support.m_userData[2] = sourceFace->m_support.m_userData[2];
        face->m_support.m_userData[3] = sourceFace->m_support.m_userData[3];
    }

    for (int childIndex = 0; childIndex < shape->m_children.m_size; ++childIndex)
    {
        const hkdShapeInstanceInfo *child =
            &((const hkdShapeInstanceInfo *)shape->m_children.m_data)[childIndex];
        fixupChild(child->m_shape, rootGeometries);
    }
}
