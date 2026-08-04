#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkGeometryMatchingUtils_Geometry.h"
#include "../headers/havok/hkGeometryMatchingUtils_TriangleMap.h"
#include "../headers/havok/hkMatrix4.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkBool.h"

/* boundary — geometry-matching core and matrix helpers (level-2 callees). */
extern void hkGeometryMatchingUtils_matchTriangles(
    const hkArray<hkGeometryMatchingUtils_Geometry> *referenceGeometries,
    const hkArray<hkGeometryMatchingUtils_Geometry> *queryGeometries,
    float maxDistance, hkGeometryMatchingUtils_TriangleMap *out); /* hkGeometryMatchingUtils::matchTriangles */
extern hkBool *hkMatrix4_isApproximatelyEqual(hkBool *result, const hkMatrix4 *a, const hkMatrix4 *b, float epsilon); /* hkMatrix4::isApproximatelyEqual (hkBool via sret) */
extern void hkMatrix4_setInverse(hkMatrix4 *out, const hkMatrix4 *in, float epsilon);
extern void hkMatrix4_transformPosition(const hkMatrix4 *m, const hkVector4 *in, hkVector4 *out);
extern void hkArrayUtil__reserve(void *array, int numElems, int sizeElem); /* hkArrayUtil::_reserve */
extern const hkMatrix4 hkMatrix4Identity;

/* hkdGeometry::setFaceIdentifiersToBestMatchingTriangle — for each face of this
   geometry, find the reference triangle (from a concatenated multi-section
   reference mesh) that best matches the face's start triangle, and record its
   original per-section vertex indices into the face's FaceIdentifier.
   The reference set is described by one Geometry over (referenceVertices,
   referenceTriangleIndices); the query set is this geometry's per-face start
   triangles, with vertices first mapped back through the inverse of this
   geometry's object transform when that transform is not identity.
   hkGeometryMatchingUtils::matchTriangles fills triangleMapOut; the per-face
   writeback then rebases each matched reference triangle's indices to its
   owning section and applies the winding rotation the match reported.
   DEVIATION: the decompiler aliases the query Geometry/array scratch onto an
   hkMatrix4 local and inlines the per-thread stack allocator; reconstructed with
   the DB-typed Geometry descriptor and explicit scratch arrays. The winding
   permutation (an inline {2,0,1} table read with a -1 underhang) is a plain
   cyclic rotation by hit.m_indexOffset. */
void hkdGeometry_setFaceIdentifiersToBestMatchingTriangle(
    hkdGeometry *self, const hkVector4 *referenceVertices,
    const int *referenceTriangleIndices, const hkArray<int> *numVerticesPerSection,
    const hkArray<int> *numTrianglesPerSection,
    hkGeometryMatchingUtils_TriangleMap *triangleMapOut, float maxDistance)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    int numFaces = self->m_faces.m_size;
    int numRefTriangles = numTrianglesPerSection->m_data[numTrianglesPerSection->m_size - 1];
    int numVertices = self->m_vertices.m_size;

    hkGeometryMatchingUtils_Geometry referenceDesc;
    hkArray<hkGeometryMatchingUtils_Geometry> referenceGeoms;
    hkGeometryMatchingUtils_Geometry queryDesc;
    hkArray<hkGeometryMatchingUtils_Geometry> queryGeoms;
    int *queryTriIndices;
    const hkVector4 *queryVertices;
    hkArray<hkVector4> transformedVerts;
    hkBool matchScratch; /* sret result holder for hkMatrix4_isApproximatelyEqual */
    int i, f;

    /* Reference geometry descriptor (single, whole concatenated mesh). */
    referenceDesc.m_vertices = referenceVertices;
    referenceDesc.m_tag = -1;
    referenceDesc.m_triangleIndices = referenceTriangleIndices;
    referenceDesc.m_numTriangles = numRefTriangles;
    referenceGeoms.m_data = &referenceDesc;
    referenceGeoms.m_size = 1;
    referenceGeoms.m_capacityAndFlags = 0x80000001; /* size 1, DONT_DEALLOCATE */

    /* Query triangle indices: the first (start) triangle of each face. */
    queryTriIndices = (int *)hkThreadMemory_allocateStack(mem, ((12 * numFaces + 16) & ~15));
    for (f = 0; f < numFaces; ++f)
    {
        const Face *face = &self->m_faces.m_data[f];
        const Triangle *tri = &self->m_triangles.m_data[face->m_startTriangleIndex];
        queryTriIndices[3 * f + 0] = tri->m_vertexIndices[0];
        queryTriIndices[3 * f + 1] = tri->m_vertexIndices[1];
        queryTriIndices[3 * f + 2] = tri->m_vertexIndices[2];
    }

    /* Query vertices: mapped through the inverse object transform unless it is
       already identity. */
    transformedVerts.m_data = 0;
    transformedVerts.m_size = 0;
    transformedVerts.m_capacityAndFlags = 0x80000000;
    if (!hkMatrix4_isApproximatelyEqual(&matchScratch, &self->m_objectIds.m_data->m_transform,
                                        &hkMatrix4Identity, 0.00000011920929f)->m_bool)
    {
        hkMatrix4 inverse;
        hkMatrix4_setInverse(&inverse, &self->m_objectIds.m_data->m_transform, 0.00000011920929f);
        if ((transformedVerts.m_capacityAndFlags & 0x3FFFFFFF) < numVertices)
        {
            int want = 2 * (transformedVerts.m_capacityAndFlags & 0x3FFFFFFF);
            if (numVertices >= want)
                want = numVertices;
            hkArrayUtil__reserve(&transformedVerts.m_data, want, 16);
        }
        transformedVerts.m_size = numVertices;
        for (i = 0; i < numVertices; ++i)
            hkMatrix4_transformPosition(&inverse, &self->m_vertices.m_data[i],
                                        &transformedVerts.m_data[i]);
        queryVertices = transformedVerts.m_data;
    }
    else
    {
        queryVertices = self->m_vertices.m_data;
    }

    queryDesc.m_vertices = queryVertices;
    queryDesc.m_tag = -1;
    queryDesc.m_triangleIndices = queryTriIndices;
    queryDesc.m_numTriangles = numFaces;
    queryGeoms.m_data = &queryDesc;
    queryGeoms.m_size = 1;
    queryGeoms.m_capacityAndFlags = 0x80000001;

    hkGeometryMatchingUtils_matchTriangles(&referenceGeoms, &queryGeoms, maxDistance, triangleMapOut);

    /* Per-face writeback of the matched reference triangle's original indices. */
    for (f = 0; f < numFaces; ++f)
    {
        const hkGeometryMatchingUtils_TriangleMap_Hit *hit = &triangleMapOut->m_foundReferenceTriangle.m_data[f];
        Face *face = &self->m_faces.m_data[f];
        int triangleIndex = hit->m_triangleIndex;

        if (triangleIndex < 0)
        {
            face->m_support.m_userData[0] = 0;
            face->m_support.m_userData[1] = 0;
            face->m_support.m_userData[2] = 0;
            face->m_support.m_userData[3] = 0;
        }
        else
        {
            int numSections = numTrianglesPerSection->m_size;
            int indexOffset = hit->m_indexOffset;
            const int *refTri = &referenceTriangleIndices[3 * triangleIndex];
            int section = 0;
            int vertexOffset;
            int s;

            for (s = 0; s < numSections; ++s)
            {
                if (numTrianglesPerSection->m_data[s] > triangleIndex)
                    break;
                ++section;
            }
            vertexOffset = (section > 0) ? numVerticesPerSection->m_data[section - 1] : 0;

            /* Cyclic winding rotation by indexOffset, rebased to the section. */
            face->m_support.m_userData[0] = (unsigned short)(refTri[indexOffset] - vertexOffset);
            face->m_support.m_userData[1] = (unsigned short)(refTri[(indexOffset + 1) % 3] - vertexOffset);
            face->m_support.m_userData[2] = (unsigned short)(refTri[(indexOffset + 2) % 3] - vertexOffset);
            face->m_support.m_userData[3] = (unsigned short)section;
        }
    }

    /* Release scratch (transformed vertices then query triangle indices). */
    if ((transformedVerts.m_capacityAndFlags & 0x80000000) == 0)
        hkThreadMemory_deallocateChunk(mem, transformedVerts.m_data,
                                       16 * transformedVerts.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    hkThreadMemory_deallocateStack(mem, queryTriIndices);
}
