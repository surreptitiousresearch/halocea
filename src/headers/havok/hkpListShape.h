#pragma once
#include "hkpShapeCollection.h"

/* hkpListShape, reproduced from the DB layout in plain-C form (self-contained).
   Layout verified against DB types_members (hkpListShape, hkpListShape::ChildInfo). */

/* hkpShapeContainer::ReferencePolicy — DB types_enum_values. */
enum hkpShapeContainerReferencePolicy
{
    REFERENCE_POLICY_IGNORE    = 0,
    REFERENCE_POLICY_INCREMENT = 1,
};

/* hkpListShape::ChildInfo (size 16). One entry per child shape. */
typedef struct hkpListShape_ChildInfo
{
    const hkpShape *m_shape;            /* 0  */
    unsigned int m_collisionFilterInfo;/* 4  */
    int m_shapeSize;                   /* 8  */
    int m_numChildShapes;              /* 12 */
} hkpListShape_ChildInfo;

/* hkArray<hkpListShape::ChildInfo> (12 bytes). */
typedef struct hkpListShape_ChildInfoArray
{
    hkpListShape_ChildInfo *m_data;    /* 0 */
    int m_size;                        /* 4 */
    int m_capacityAndFlags;            /* 8 */
} hkpListShape_ChildInfoArray;

/* hkpListShape (size 112). Base hkpShapeCollection then the child array, disabled
   bookkeeping, the cached local AABB (16-byte aligned hkVector4s reproduced as
   float[4]), and the enabled-children bitset. */
typedef struct hkpListShape
{
    hkpShapeCollection base;              /* 0  */
    hkpListShape_ChildInfoArray m_childInfo; /* 24 */
    unsigned short m_flags;              /* 36 */
    unsigned short m_numDisabledChildren;/* 38 */
    unsigned char _pad40[8];             /* 40 */
    float m_aabbHalfExtents[4];          /* 48 */
    float m_aabbCenter[4];               /* 64 */
    unsigned int m_enabledChildren[8];   /* 80 */
} hkpListShape;
