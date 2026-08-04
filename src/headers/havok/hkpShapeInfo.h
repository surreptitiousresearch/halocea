#pragma once
#include "hkReferencedObject.h"
#include "hkBool.h"
#include "hkArray.h"
#include "hkTransform.h"
#include "hkpShape.h"
#include "hkRefPtr.h"

/* size 112. hkpShapeInfo — a physics shape plus the per-child mesh names and
   transforms used when matching graphics meshes to a compound/list physics
   shape. Layout from DB types_members. */
struct hkpShapeInfo : hkReferencedObject
{
    hkRefPtr<const hkpShape> m_shape;        /* 8  */
    hkBool m_isHierarchicalCompound;         /* 12 */
    hkBool m_hkdShapesCollected;             /* 13 */
    unsigned char _pad0E[2];                 /* 14 alignment */
    hkArray<const char *> m_childShapeNames; /* 16 */
    hkArray<hkTransform> m_childTransforms;  /* 28 */
    unsigned char _pad28[8];                 /* 40 alignment to 16 */
    hkTransform m_transform;                 /* 48 -> 112 */
};
typedef struct hkpShapeInfo hkpShapeInfo;
