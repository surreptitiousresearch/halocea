#include "../headers/havok/hkMeshSection_boundary.h"
#include "../headers/havok/hkMeshShape.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkReferencedObject.h"

/* boundary — hkMeshShape section lock/unlock (virtual slots 0x10/0x14) and the
   vertex-buffer unlock + array reserve helpers (level-2 callees). */
extern void hkMeshShape_lockSection(const hkMeshShape *self, int sectionIndex,
                                    unsigned char accessFlags, hkMeshSection *out); /* virtual lockSection */
extern void hkMeshShape_unlockSection(const hkMeshShape *self, const hkMeshSection *section); /* virtual unlockSection */
extern void hkMeshVertexBuffer_unlock(hkMeshVertexBuffer *self, const hkMeshVertexBuffer_LockedVertices *locked); /* virtual unlock */
extern void hkArrayUtil__reserve(void *array, int numElems, int sizeElem);          /* hkArrayUtil::_reserve */

/* hkMeshTohkGeometryConverter::getVerticesIndicesFromMeshSection — append one
   mesh section's triangle indices and positions to the running vertex/index
   arrays. The section is locked to grab its vertex buffer, the POSITION element
   is partial-locked, the section's triangle indices are appended (rebased by the
   current vertex count), and the positions are read out as an hkVector4 array.
   DEVIATION: the scratch LockInput / PartialLockInput / LockedVertices structs
   are built inline via raw stores in the original; reconstructed here with the
   DB-typed structures. Reserve growth mirrors the inlined "double, else need". */
void hkMeshTohkGeometryConverter_getVerticesIndicesFromMeshSection(
    const hkMeshShape *shape, int sectionIndex,
    hkArray<hkVector4> *vertices, hkArray<int> *indices)
{
    hkMeshSection section;
    hkMeshVertexBuffer *vb;
    hkVertexFormat format;
    int posElem;

    hkMeshShape_lockSection(shape, sectionIndex, 3, &section);
    vb = section.m_vertexBuffer;
    if (vb)
        hkReferencedObject_addReference((hkReferencedObject *)vb);
    hkMeshShape_unlockSection(shape, &section);

    hkVertexFormat_construct(&format);
    hkMeshVertexBuffer_getVertexFormat(vb, &format);
    posElem = hkVertexFormat_findElementIndex(&format, USAGE_POSITION, 0);

    if (posElem >= 0)
    {
        hkMeshVertexBuffer_LockInput lockInput;
        hkMeshVertexBuffer_PartialLockInput partial;
        hkMeshVertexBuffer_LockedVertices locked;

        lockInput.m_startVertex = 0;
        lockInput.m_numVertices = -1;
        lockInput.m_noWait.m_bool = 0;          /* hkBool wraps a char (m_bool) */
        lockInput.m_contiguousAccess.m_bool = 0;
        lockInput.m_lockFlags = 3;

        partial.m_numLockFlags = 1;
        partial.m_elementIndices[0] = posElem;
        partial.m_lockFlags[0] = 9;

        if (hkMeshVertexBuffer_partialLock(vb, &lockInput, &partial, &locked) == RESULT_SUCCESS)
        {
            hkArray<> triIndices;
            hkThreadMemory *mem = hkThreadMemory_getCurrent();
            int oldIndexCount = indices->m_size;
            int vertexBase = vertices->m_size;
            int newIndexCount;
            int i;
            int vertexStart;

            triIndices.m_data = 0;
            triIndices.m_size = 0;
            triIndices.m_capacityAndFlags = 0x80000000;
            hkMeshPrimitiveUtil_appendTriangleIndices(&section, &triIndices);

            newIndexCount = oldIndexCount + triIndices.m_size;
            if ((indices->m_capacityAndFlags & 0x3FFFFFFF) < newIndexCount)
            {
                int want = 2 * (indices->m_capacityAndFlags & 0x3FFFFFFF);
                if (newIndexCount >= want)
                    want = newIndexCount;
                hkArrayUtil__reserve(&indices->m_data, want, 4);
            }
            indices->m_size = newIndexCount;
            /* appendTriangleIndices fills an unsigned-short index array. */
            for (i = 0; i < triIndices.m_size; ++i)
                indices->m_data[oldIndexCount + i] = ((unsigned short *)triIndices.m_data)[i] + vertexBase;

            vertexStart = vertices->m_size;
            {
                int newVertexCount = vertexStart + locked.m_numVertices;
                if ((vertices->m_capacityAndFlags & 0x3FFFFFFF) < newVertexCount)
                {
                    int want = 2 * (vertices->m_capacityAndFlags & 0x3FFFFFFF);
                    if (newVertexCount >= want)
                        want = newVertexCount;
                    hkArrayUtil__reserve(&vertices->m_data, want, 16);
                }
                vertices->m_size = newVertexCount;
            }
            hkMeshVertexBuffer_getElementVectorArray(vb, &locked, 0, &vertices->m_data[vertexStart]);
            hkMeshVertexBuffer_unlock(vb, &locked);

            if (triIndices.m_capacityAndFlags >= 0)
                hkThreadMemory_deallocateChunk(mem, triIndices.m_data,
                                               (2 * triIndices.m_capacityAndFlags) & 0x7FFFFFFE,
                                               HK_MEMORY_CLASS_ARRAY);
        }
    }

    hkReferencedObject_removeReference((hkReferencedObject *)vb);
}
