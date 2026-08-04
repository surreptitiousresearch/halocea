#include "../headers/havok/hkMesh_boundary.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkReferencedObject.h"

/* Build a mesh section for the back-facing half of a plane split: a position-only
   vertex format (upgraded to the material's compatible format when a material is
   supplied), a vertex buffer filled from backVertices, then a triangle-list
   section from backIndices submitted to the section builder. */
void hkdMeshSplittingPlaneUtil_addBackFaces(hkMeshSystem *meshSystem,
                                            const hkArray<hkVector4> *backVertices,
                                            const hkArray<unsigned short> *backIndices,
                                            hkMeshMaterial *backMaterial,
                                            hkMeshSectionBuilder *builder)
{
    hkVertexFormat positionFormat;
    hkVertexFormat bufferFormat;
    hkMeshVertexBuffer_LockInput lockInput;
    hkMeshVertexBuffer_LockedVertices locked;
    hkMeshVertexBuffer *vertexBuffer;

    hkVertexFormat_construct(&bufferFormat);
    hkVertexFormat_construct(&positionFormat);
    hkVertexFormat_addElement(&positionFormat, USAGE_POSITION, 10, 3, 3); /* DataType 10 */
    hkVertexFormat_makeCanonicalOrder(&positionFormat);

    if (backMaterial)
        hkMeshMaterial_createCompatibleVertexFormat(backMaterial, &positionFormat, &bufferFormat);
    else
        hkVertexFormat_assign(&bufferFormat, &positionFormat);

    vertexBuffer = hkMeshSystem_createVertexBuffer(meshSystem, &bufferFormat, backVertices->m_size);

    lockInput.m_startVertex = 0;
    lockInput.m_numVertices = -1;
    lockInput.m_noWait.m_bool = 0;          /* hkBool wraps a char (m_bool) */
    lockInput.m_contiguousAccess.m_bool = 0;
    lockInput.m_lockFlags = 14;

    hkMeshVertexBuffer_lock(vertexBuffer, &lockInput, &locked);
    hkMeshVertexBuffer_setElementVectorArray(vertexBuffer, &locked, 0, backVertices->m_data);
    hkMeshVertexBuffer_unlock(vertexBuffer, &locked);

    hkMeshSectionBuilder_startMeshSection(builder, vertexBuffer, backMaterial);
    hkReferencedObject_removeReference((hkReferencedObject *)vertexBuffer);
    hkMeshSectionBuilder_concatPrimitives(builder, PRIMITIVE_TYPE_TRIANGLE_LIST,
                                          (const unsigned short *)backIndices->m_data, backIndices->m_size, 0);
    hkMeshSectionBuilder_endMeshSection(builder);
}
