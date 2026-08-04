#pragma once
#include "hkTransform.h"
#include "hkArray.h"

typedef struct hkdBreakableShape hkdBreakableShape;

/* types_members hkdShapeInstanceInfo::RuntimeInfo size 8 */
typedef struct hkdShapeInstanceInfo_RuntimeInfo
{
    float m_distanceToDestructionPoint; /* 0 */
    unsigned short m_oldChildIdx;       /* 4 */
    unsigned char m_forceFixed;         /* 6 */
    unsigned char _pad07;               /* 7 */
} hkdShapeInstanceInfo_RuntimeInfo;

/* Local alias so member spellings match the DB's nested-qualified type. */
typedef hkdShapeInstanceInfo_RuntimeInfo RuntimeInfo;

/* size 80 */
typedef struct hkdShapeInstanceInfo
{
    hkTransform m_transform;          /* 0  */
    const hkdBreakableShape *m_shape; /* 64 */
    unsigned char m_damage;           /* 68 */
    unsigned char _pad45[3];          /* 69 */
    RuntimeInfo m_runtimeInfo;        /* 72 */
} hkdShapeInstanceInfo;

/* hkArray<hkdShapeInstanceInfo>, 12 bytes — legacy spelling kept for callers. */
typedef hkArray<hkdShapeInstanceInfo> hkArray_hkdShapeInstanceInfo;
