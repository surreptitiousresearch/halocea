#pragma once

#include "hkPadSpu.h"
#include "hkVector4.h"

typedef struct hkpCollisionDispatcher hkpCollisionDispatcher;
typedef struct hkpShapeCollectionFilter hkpShapeCollectionFilter;
typedef struct hkpConvexListFilter hkpConvexListFilter;

/* types_members hkpCollisionInput::Aabb32Info (48 bytes) */
typedef struct hkpCollisionInput_Aabb32Info
{
    hkVector4 m_bitOffsetLow;  /* 0x00 */
    hkVector4 m_bitOffsetHigh; /* 0x10 */
    hkVector4 m_bitScale;      /* 0x20 */
} hkpCollisionInput_Aabb32Info;

/* Local alias so member spellings match the DB's nested-qualified type. */
typedef hkpCollisionInput_Aabb32Info Aabb32Info;

/* hkpCollisionInput, size 80. Layout from DB types_members. */
typedef struct hkpCollisionInput
{
    hkPadSpu<hkpCollisionDispatcher *> m_dispatcher;            /* 0  */
    hkPadSpu<float> m_tolerance;                                /* 4  */
    hkPadSpu<const hkpShapeCollectionFilter *> m_filter;        /* 8  */
    hkPadSpu<const hkpConvexListFilter *> m_convexListFilter;   /* 12 */
    hkPadSpu<int> m_createPredictiveAgents;                     /* 16 */
    unsigned char _pad14[12];                                   /* 20 */
    Aabb32Info m_aabb32Info;                                    /* 32 hkpCollisionInput::Aabb32Info -> 80 */
} hkpCollisionInput;
