#pragma once
#include "hkpShape.h"
#include "hkTransform.h"
#include "hkpConvexTransformShapeBase.h"

/* hkpConvexTransformShape (96 bytes) — a convex shape wrapped in a rigid
   transform. The leading 32 bytes are the hkpConvexTransformShapeBase sub-object
   (shape base + child container + radius), populated by the base constructor;
   the trailing 64 bytes hold the transform. The base layout now lives in
   hkpConvexTransformShapeBase.h (canonical, full field set). */
typedef struct hkpConvexTransformShape
{
    hkpConvexTransformShapeBase base; /* 0x00 */
    hkTransform m_transform;          /* 0x20 */
} hkpConvexTransformShape;

void hkpConvexTransformShape_construct(hkpConvexTransformShape *self, const hkpShape *childShape,
                                       const hkTransform *transform, int referencePolicy); /* hkpConvexTransformShape::hkpConvexTransformShape */
