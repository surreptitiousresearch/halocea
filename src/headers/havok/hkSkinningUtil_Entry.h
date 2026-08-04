#pragma once
#include "hkArray.h"

/* hkSkinningUtil::Entry, size 8 (DB types_members). One candidate bone binding:
   the squared distance from the vertex to the bone and the bone (matrix) index
   (a negative index marks an unused slot). */
typedef struct hkSkinningUtil_Entry
{
    float m_distanceSquared; /* 0 */
    int m_index;             /* 4 */
} hkSkinningUtil_Entry;

/* Local alias matching the DB's nested-qualified spelling. */
typedef hkSkinningUtil_Entry Entry_hkSkinningUtil;
