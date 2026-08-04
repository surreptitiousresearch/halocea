#pragma once
#include "hkdBreakableShape.h"
#include "hkBool.h"

/* hkdCompoundBreakableShape : hkdBreakableShape, size 224. Verified against DB
   types_members. Reached here only through its hkdBreakableShape base; the extra
   fields are present for layout. */
typedef struct hkdCompoundBreakableShape
{
    hkdBreakableShape base;                     /* 0   (208) */
    const hkdBreakableShape *m_rootBreakableShape;/* 208 */
    hkBool m_useChildrenBreakableThreshold;     /* 212 */
    unsigned char _padD5[3];                    /* 213 (alignment) */
    int m_unusedPaddingCompound[2];             /* 216 -> 224 */
} hkdCompoundBreakableShape;
