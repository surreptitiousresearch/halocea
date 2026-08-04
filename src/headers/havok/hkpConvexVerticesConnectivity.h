#pragma once
#include "hkReferencedObject.h"
#include "hkArray.h"

/* hkpConvexVerticesConnectivity (size 32, DB types_members) — face/vertex adjacency
   for a convex-vertices shape. m_vertexIndices is the concatenation of every face's
   ordered vertex loop; m_numVerticesPerFace gives each face's loop length so the
   flat index list can be partitioned back into faces. */
typedef struct hkpConvexVerticesConnectivity
{
    hkReferencedObject base;                    /* 0  vtable + refcount */
    hkArray<unsigned short> m_vertexIndices;    /* 8  */
    hkArray<unsigned char>  m_numVerticesPerFace;/* 20 */
} hkpConvexVerticesConnectivity;
