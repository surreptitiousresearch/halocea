#pragma once
#include "hkReferencedObject.h"
#include "hkBool.h"
#include "hkArray.h"
#include "hkEnum.h"
#include "hkRefPtr.h"

typedef struct hkdFracture hkdFracture;
typedef struct hkxAttribute hkxAttribute;

/* types_enum_values hkdShape::Connectivity */
enum Connectivity
{
    CONNECTIVITY_INHERITED = 0,
    CONNECTIVITY_PARTIAL = 1,
    CONNECTIVITY_FULL = 2,
    CONNECTIVITY_NONE = 3,
};

/* types_enum_values hkdShape::FlexibleJointType */
enum FlexibleJointType
{
    FLEXJOINT_SELF = 0,
    FLEXJOINT_CHILDREN = 1,
    FLEXJOINT_FIXED = 2,
};

/* types_enum_values hkdShape::IntegrityType */
enum IntegrityType
{
    INTEGRITY_INHERITED = 0,
    INTEGRITY_NONE = 1,
    INTEGRITY_ON = 2,
    INTEGRITY_CHILDREN = 3,
    INTEGRITY_FIXED = 4,
};

/* types_enum_values hkdShape::BodyQualityType */
enum BodyQualityType
{
    QUALITY_INHERITED = 0,
    QUALITY_DEBRIS = 1,
    QUALITY_DEBRIS_SIMPLE_TOI = 2,
    QUALITY_MOVING = 3,
    QUALITY_CRITICAL = 4,
};

/* size 60. Layout from DB types_members hkdShape. */
struct hkdShape : hkReferencedObject
{
    const char *m_parentName;                                        /* 8  */
    hkRefPtr<hkdFracture> m_fracture;                                /* 12 */
    hkBool m_mergeCoplanarTriangles;                                 /* 16 */
    hkEnum<Connectivity, unsigned char> m_connectivity;              /* 17 */
    hkBool m_flattenChildCompounds;                                  /* 18 */
    unsigned char _pad13;                                            /* 19 */
    hkRefPtr<hkdFracture> m_dynamicFracture;                         /* 20 */
    float m_strength;                                                /* 24 */
    float m_relativeSubpieceStrength;                                /* 28 */
    float m_breakingPropagationRate;                                 /* 32 */
    float m_destructionRadius;                                       /* 36 */
    hkEnum<BodyQualityType, unsigned char> m_bodyQualityType;        /* 40 */
    hkEnum<FlexibleJointType, unsigned char> m_flexibleJointType;    /* 41 */
    hkEnum<IntegrityType, unsigned char> m_integrityType;            /* 42 */
    unsigned char _pad2B;                                            /* 43 */
    float m_tensionLength;                                           /* 44 */
    hkArray<hkxAttribute> m_attributes;                              /* 48 */
};
typedef struct hkdShape hkdShape;
