#pragma once
#include "hkArray.h"
#include "hkVertexFormat.h"
#include "hkMesh_boundary.h"
#include "hkMeshSection.h"
#include "hkVector4.h"
#include "hkPointerMap.h"

typedef struct hkMeshShape hkMeshShape;
typedef struct hkFindUniquePositionsUtil hkFindUniquePositionsUtil;

/* types_members hkMeshSectionLockSet, size 24 */
typedef struct hkMeshSectionLockSet
{
    hkArray<hkMeshSection> m_sections;      /* 0x00 */
    hkArray<const hkMeshShape *> m_shapes;  /* 0x0C */
} hkMeshSectionLockSet;

/* hkPointerMap<unsigned int,int> (12 bytes) — see hkPointerMap.h for the
   DB-verified base layout. */
typedef hkPointerMap<unsigned int, int> hkPointerMap_uint_int;

/* types_members hkMeshVertexBuffer::PartialLockInput, size 164 */
typedef struct hkMeshVertexBuffer_PartialLockInput
{
    int m_numLockFlags;             /* 0x00 */
    int m_elementIndices[32];       /* 0x04 element index list */
    unsigned char m_lockFlags[32];  /* 0x84 */
} hkMeshVertexBuffer_PartialLockInput;

extern const int RESULT_SUCCESS;

int  hkMeshShape_getNumSections(const hkMeshShape *self);                                        /* virtual getNumSections */
void hkMeshSectionLockSet_addMeshSections(hkMeshSectionLockSet *self, const hkMeshShape *shape, unsigned int accessFlags); /* addMeshSections */
void hkMeshSectionLockSet_destruct(hkMeshSectionLockSet *self);                                  /* ~hkMeshSectionLockSet */
void hkMeshVertexBuffer_getVertexFormat(hkMeshVertexBuffer *self, hkVertexFormat *out);          /* virtual getVertexFormat */
int  hkVertexFormat_findElementIndex(const hkVertexFormat *self, int usage, int subUsage);       /* findElementIndex */
int  hkMeshVertexBuffer_partialLock(hkMeshVertexBuffer *self, const hkMeshVertexBuffer_LockInput *input,
                                    const hkMeshVertexBuffer_PartialLockInput *partial, hkMeshVertexBuffer_LockedVertices *out); /* virtual partialLock */
void hkMeshVertexBuffer_getElementVectorArray(hkMeshVertexBuffer *self, const hkMeshVertexBuffer_LockedVertices *locked, int elementIndex, hkVector4 *out); /* virtual */
void hkMeshPrimitiveUtil_appendTriangleIndices(const hkMeshSection *section, hkArray<> *indicesOut); /* hkMeshPrimitiveUtil::appendTriangleIndices */
int  hkFindUniquePositionsUtil_addPosition(hkFindUniquePositionsUtil *self, const hkVector4 *position); /* addPosition */
void hkPointerMap_insert(void *map, unsigned int key, unsigned int value);                       /* hkPointerMapBase::insert */
