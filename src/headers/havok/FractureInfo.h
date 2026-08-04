#pragma once
#include "hkdFracture.h"

/* size 8. Pair of fracture definitions consulted when deciding whether to build
   a deformable/dynamic breakable shape. Layout from DB type FractureInfo. */
typedef struct FractureInfo
{
    hkdFracture *m_fracture;        /* 0 */
    hkdFracture *m_dynamicFracture; /* 4 */
} FractureInfo;
