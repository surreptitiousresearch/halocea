#pragma once
#include "hkpLinearCastCollisionInput.h"
#include "hkdShapeInstanceInfo.h"
#include "hkInplaceArrayAligned16.h"

/* hkdDecorateFractureFaceActionInternalInfo, size 5296. Per-run scratch state
   for the decorate-fracture-face action: the linear-cast collision input plus
   the four accumulated decoration lists (top/bottom/left/right faces).
   Layout from DB types_members. */
typedef struct hkdDecorateFractureFaceActionInternalInfo
{
    hkpLinearCastCollisionInput m_collisionInput;                          /* 0    (112) */
    hkInplaceArrayAligned16<hkdShapeInstanceInfo, 16> m_topDecorations;    /* 112  (1296) */
    hkInplaceArrayAligned16<hkdShapeInstanceInfo, 16> m_bottomDecorations; /* 1408 */
    hkInplaceArrayAligned16<hkdShapeInstanceInfo, 16> m_leftDecorations;   /* 2704 */
    hkInplaceArrayAligned16<hkdShapeInstanceInfo, 16> m_rightDecorations;  /* 4000 -> 5296 */
} hkdDecorateFractureFaceActionInternalInfo;
