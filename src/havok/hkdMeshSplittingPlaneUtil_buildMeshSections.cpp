#include "../headers/havok/hkMeshSection_boundary.h"
#include "../headers/havok/hkMeshShape.h"
#include "../headers/havok/hkThreadMemory.h"

extern void hkArrayUtil__reserve(void *array, int count, int elementSize);  /* hkArrayUtil::_reserve */
extern void hkArrayUtil__reserveMore(void *array, int elementSize);         /* hkArrayUtil::_reserveMore */

/* Rebuild the mesh sections of a split ("plane") shape while recording, for the
   caller, the unique-position remapping and the directed half-edges of every
   triangle. For each section: lock its positions, copy the triangle indices,
   re-emit the section into the builder, then for each triangle register its
   three corners with the unique-position util and insert its three directed
   edges (packed (from<<16)|(to+1)) into edgeMap; optionally stream the remapped
   corner indices into indicesOut. Returns 0 on success, 1 on a lock/format
   failure. (Opaque boundary struct sizes are stack-derived.) */
int hkdMeshSplittingPlaneUtil_buildMeshSections(const hkMeshShape *planeShape, hkFindUniquePositionsUtil *posUtil,
                                                hkPointerMap_uint_int *edgeMap, hkArray<unsigned short> *indicesOut,
                                                hkMeshSectionBuilder *builder)
{
    hkMeshSectionLockSet lockSet;
    hkArray<unsigned short> triangleIndices;
    hkArray<hkVector4> positions;
    hkMeshSection *sections;
    int numSections;
    int result = 0;
    int i;

    if (indicesOut)
        indicesOut->m_size = 0;

    lockSet.m_sections.m_data = 0; lockSet.m_sections.m_size = 0; lockSet.m_sections.m_capacityAndFlags = 0x80000000;
    lockSet.m_shapes.m_data = 0;   lockSet.m_shapes.m_size = 0;   lockSet.m_shapes.m_capacityAndFlags = 0x80000000;
    hkMeshSectionLockSet_addMeshSections(&lockSet, planeShape, ACCESS_INDICES | ACCESS_VERTEX_BUFFER);

    numSections = hkMeshShape_getNumSections(planeShape);
    triangleIndices.m_data = 0; triangleIndices.m_size = 0; triangleIndices.m_capacityAndFlags = 0x80000000;
    positions.m_data = 0;       positions.m_size = 0;       positions.m_capacityAndFlags = 0x80000000;
    sections = (hkMeshSection *)lockSet.m_sections.m_data;

    for (i = 0; i < numSections; ++i)
    {
        const hkMeshSection *section = &sections[i];
        hkMeshVertexBuffer *vertexBuffer = section->m_vertexBuffer;
        hkMeshMaterial *material = section->m_material;
        hkVertexFormat format;
        hkMeshVertexBuffer_LockInput lockInput;
        hkMeshVertexBuffer_PartialLockInput partialInput;
        hkMeshVertexBuffer_LockedVertices locked;
        const unsigned short *triIndices;
        int elementIndex, numVertices, t;

        lockInput.m_startVertex = 0; lockInput.m_numVertices = -1;
        lockInput.m_noWait.m_bool = 0; lockInput.m_contiguousAccess.m_bool = 0; lockInput.m_lockFlags = 3; /* hkBool wraps a char */
        partialInput.m_numLockFlags = 0;

        hkVertexFormat_construct(&format);
        hkMeshVertexBuffer_getVertexFormat(vertexBuffer, &format);
        elementIndex = hkVertexFormat_findElementIndex(&format, USAGE_POSITION, 0);
        if (elementIndex < 0)
        {
            result = 1;
            break;
        }

        partialInput.m_elementIndices[0] = elementIndex;
        partialInput.m_numLockFlags = 1;

        if (hkMeshVertexBuffer_partialLock(vertexBuffer, &lockInput, &partialInput, &locked) != RESULT_SUCCESS)
        {
            result = 1;
            break;
        }

        numVertices = locked.m_numVertices; /* typed member @516 (hkMesh_boundary.h) */
        if ((positions.m_capacityAndFlags & 0x3FFFFFFF) < numVertices)
        {
            int cap = numVertices;
            if (numVertices < 2 * (positions.m_capacityAndFlags & 0x3FFFFFFF))
                cap = 2 * (positions.m_capacityAndFlags & 0x3FFFFFFF);
            hkArrayUtil__reserve(&positions, cap, 16);
        }
        positions.m_size = numVertices;

        hkMeshVertexBuffer_getElementVectorArray(vertexBuffer, &locked, 0, (hkVector4 *)positions.m_data);
        hkMeshVertexBuffer_unlock(vertexBuffer, &locked);

        triangleIndices.m_size = 0;
        hkMeshPrimitiveUtil_appendTriangleIndices(section, (hkArray<void> *)&triangleIndices); /* type-erased array boundary */

        hkMeshSectionBuilder_startMeshSection(builder, vertexBuffer, material);
        hkMeshSectionBuilder_concatPrimitives(builder, PRIMITIVE_TYPE_TRIANGLE_LIST,
                                              (const unsigned short *)triangleIndices.m_data, triangleIndices.m_size, 0);
        hkMeshSectionBuilder_endMeshSection(builder);

        triIndices = (const unsigned short *)triangleIndices.m_data;
        for (t = 0; t + 2 < triangleIndices.m_size; t += 3)
        {
            const hkVector4 *verts = (const hkVector4 *)positions.m_data;
            int a = hkFindUniquePositionsUtil_addPosition(posUtil, &verts[triIndices[t]]);
            int b = hkFindUniquePositionsUtil_addPosition(posUtil, &verts[triIndices[t + 1]]);
            int c = hkFindUniquePositionsUtil_addPosition(posUtil, &verts[triIndices[t + 2]]);

            hkPointerMap_insert(&edgeMap->m_map, (a << 16) | (b + 1), 1u);
            hkPointerMap_insert(&edgeMap->m_map, (c + 1) | (b << 16), 1u);
            hkPointerMap_insert(&edgeMap->m_map, (c << 16) | (a + 1), 1u);

            if (indicesOut)
            {
                if (indicesOut->m_size == (indicesOut->m_capacityAndFlags & 0x3FFFFFFF))
                    hkArrayUtil__reserveMore(indicesOut, 2);
                ((unsigned short *)indicesOut->m_data)[indicesOut->m_size++] = (unsigned short)a;
                if (indicesOut->m_size == (indicesOut->m_capacityAndFlags & 0x3FFFFFFF))
                    hkArrayUtil__reserveMore(indicesOut, 2);
                ((unsigned short *)indicesOut->m_data)[indicesOut->m_size++] = (unsigned short)b;
                if (indicesOut->m_size == (indicesOut->m_capacityAndFlags & 0x3FFFFFFF))
                    hkArrayUtil__reserveMore(indicesOut, 2);
                ((unsigned short *)indicesOut->m_data)[indicesOut->m_size++] = (unsigned short)c;
            }
        }
    }

    if ((int)positions.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), positions.m_data,
                                       16 * positions.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    if ((int)triangleIndices.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), triangleIndices.m_data,
                                       (2 * triangleIndices.m_capacityAndFlags) & 0x7FFFFFFE, HK_MEMORY_CLASS_ARRAY);
    hkMeshSectionLockSet_destruct(&lockSet);
    return result;
}
