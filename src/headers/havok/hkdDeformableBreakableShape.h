#pragma once
#include "hkdCompoundBreakableShape.h"
#include "hkArray.h"
#include "hkTransform.h"

/* hkdDeformableBreakableShape : hkdCompoundBreakableShape, size 236 (allocated
   240). Verified against DB types_members. Callers treat it through its
   hkdBreakableShape base for field access. */
typedef struct hkdDeformableBreakableShape
{
    hkdCompoundBreakableShape base;              /* 0   (224) */
    hkArray<hkTransform> m_origChildTransforms;  /* 224 -> 236 */
} hkdDeformableBreakableShape;
