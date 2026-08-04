#pragma once
#include "hkdVector2.h"
/* hkdConvex2dPolygonList (544 bytes) — DB types_members:
     m_vertices    hkInplaceArray<hkdVector2,64>   offset 0,   size 524
     m_numVertices hkInplaceArray<unsigned char,8> offset 524, size 20
   Reproduced flat (the hkInplaceArray<T,N> template header is C++-only): each
   inline array is { T* data; int size; int capacityAndFlags; T storage[N]; }. */
typedef struct hkdConvex2dPolygonList
{
    /* m_vertices : hkInplaceArray<hkdVector2,64> */
    hkdVector2 *m_vertices_data;              /* 0   */
    int         m_vertices_size;             /* 4   */
    int         m_vertices_capacityAndFlags; /* 8   */
    hkdVector2  m_vertices_storage[64];      /* 12  */
    /* m_numVertices : hkInplaceArray<unsigned char,8> */
    unsigned char *m_numVertices_data;              /* 524 */
    int            m_numVertices_size;             /* 528 */
    int            m_numVertices_capacityAndFlags; /* 532 */
    unsigned char  m_numVertices_storage[8];       /* 536 */
} hkdConvex2dPolygonList;
