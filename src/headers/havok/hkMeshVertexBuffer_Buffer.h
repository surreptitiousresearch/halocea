#pragma once
#include "hkVertexFormat.h"

/* types_members hkMeshVertexBuffer::LockedVertices::Buffer (size 16) — a single
   locked vertex stream: base pointer, byte stride between consecutive vertices,
   and the element descriptor (data type + component count) it holds. */
typedef struct hkMeshVertexBuffer_LockedVertices_Buffer
{
    void *m_start;              /* 0x00 */
    int   m_stride;            /* 0x04 */
    hkVertexFormat_Element m_element; /* 0x08 (hkVertexFormat::Element) */
} hkMeshVertexBuffer_LockedVertices_Buffer;
