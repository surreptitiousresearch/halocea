#pragma once
#include "hkVertexFormat.h"
#include "hkBool.h"

typedef struct hkMeshSystem hkMeshSystem;
typedef struct hkMeshVertexBuffer hkMeshVertexBuffer;
typedef struct hkMeshMaterial hkMeshMaterial;
typedef struct hkMeshSectionBuilder hkMeshSectionBuilder;

/* types_members hkMeshVertexBuffer::LockInput, size 16 */
typedef struct hkMeshVertexBuffer_LockInput
{
    int m_startVertex;            /* 0  */
    int m_numVertices;            /* 4  */
    hkBool m_noWait;              /* 8  */
    hkBool m_contiguousAccess;    /* 9  */
    unsigned char _pad0A[2];      /* 10 */
    int m_lockFlags;              /* 12 */
} hkMeshVertexBuffer_LockInput;

/* types_members hkMeshVertexBuffer::LockedVertices::Buffer size 16 */
typedef struct hkMeshVertexBuffer_LockedVertices_Buffer
{
    void *m_start;                    /* 0  */
    int m_stride;                     /* 4  */
    Element m_element;                /* 8  hkVertexFormat::Element */
} hkMeshVertexBuffer_LockedVertices_Buffer;

/* Local alias so member spellings match the DB's nested-qualified type
   (hkMeshVertexBuffer::LockedVertices::Buffer). */
typedef hkMeshVertexBuffer_LockedVertices_Buffer Buffer;

/* types_members hkMeshVertexBuffer::LockedVertices, size 524 */
typedef struct hkMeshVertexBuffer_LockedVertices
{
    Buffer m_buffers[32];    /* 0    */
    int m_numBuffers;        /* 512  */
    int m_numVertices;       /* 516  */
    hkBool m_isInterleaved;  /* 520  */
    unsigned char _pad209[3];/* 521  */
} hkMeshVertexBuffer_LockedVertices;

/* USAGE_POSITION comes from hkVertexFormat.h (DataUsage);
   PRIMITIVE_TYPE_TRIANGLE_LIST from hkMeshSection.h (PrimitiveType). */
#include "hkMeshSection.h"

void hkVertexFormat_construct(hkVertexFormat *self);                                                        /* hkVertexFormat::hkVertexFormat */
void hkVertexFormat_addElement(hkVertexFormat *self, int usage, int dataType, int numValues, int subUsage); /* hkVertexFormat::addElement */
void hkVertexFormat_makeCanonicalOrder(hkVertexFormat *self);
void hkVertexFormat_assign(hkVertexFormat *dst, const hkVertexFormat *src);                                 /* operator= */

/* virtual dispatch on the opaque mesh objects */
void hkMeshMaterial_createCompatibleVertexFormat(hkMeshMaterial *self, const hkVertexFormat *in, hkVertexFormat *out);
hkMeshVertexBuffer *hkMeshSystem_createVertexBuffer(hkMeshSystem *self, const hkVertexFormat *format, int numVertices);
void hkMeshVertexBuffer_lock(hkMeshVertexBuffer *self, const hkMeshVertexBuffer_LockInput *input, hkMeshVertexBuffer_LockedVertices *out);
void hkMeshVertexBuffer_setElementVectorArray(hkMeshVertexBuffer *self, hkMeshVertexBuffer_LockedVertices *locked, int elementIndex, const void *data);
void hkMeshVertexBuffer_unlock(hkMeshVertexBuffer *self, hkMeshVertexBuffer_LockedVertices *locked);

void hkMeshSectionBuilder_startMeshSection(hkMeshSectionBuilder *self, hkMeshVertexBuffer *vertexBuffer, hkMeshMaterial *material);
void hkMeshSectionBuilder_concatPrimitives(hkMeshSectionBuilder *self, int primitiveType, const unsigned short *indices, int count, int base);
void hkMeshSectionBuilder_endMeshSection(hkMeshSectionBuilder *self);
