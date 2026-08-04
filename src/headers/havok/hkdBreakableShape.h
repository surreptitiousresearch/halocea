#pragma once
#include "hkReferencedObject.h"
#include "hkArray.h"
#include "hkEnum.h"
#include "hkRefPtr.h"
#include "hkpShape.h"
#include "hkpMassProperties.h"
#include "hkdShape.h"
#include "hkdBreakableBody.h"

typedef struct hkdGeometry hkdGeometry;
typedef struct hkdGraphicsShape hkdGraphicsShape;
typedef struct hkdShapeInstanceInfo hkdShapeInstanceInfo;
typedef struct hkdAction hkdAction;
typedef struct hkdBreakableShape_Connection hkdBreakableShape_Connection; /* hkdBreakableShape::Connection */

/* types_enum_values hkdBreakableShape::ShapeType */
enum ShapeType
{
    SHAPE_TYPE_INVALID = 0,
    SHAPE_TYPE_SIMPLE = 1,
    SHAPE_TYPE_COMPOUND = 2,
    SHAPE_TYPE_DEFORMABLE = 3,
    SHAPE_TYPE_NUM_TYPES = 4,
};

/* types_enum_values hkdBreakableShape::Flags — MASK value, use directly */
enum hkdBreakableShape_Flags
{
    FLAG_DO_NOT_FLATTEN_RECURSIVE_COMPOUNDS = 2,
};

/* Local alias so member spellings match the DB's nested-qualified template
   argument (hkArray<hkdBreakableShape::Connection>). */
typedef hkdBreakableShape_Connection Connection;

/* size 208. Layout verified against DB types_members hkdBreakableShape. */
struct hkdBreakableShape : hkReferencedObject
{
    hkEnum<ShapeType, unsigned char> m_type;                       /* 8   */
    hkEnum<Connectivity, unsigned char> m_connectivityType;        /* 9   hkdShape::Connectivity */
    hkEnum<FlexibleJointType, unsigned char> m_flexibleJointType;  /* 10  hkdShape::FlexibleJointType */
    hkEnum<IntegrityType, unsigned char> m_integrityType;          /* 11  hkdShape::IntegrityType */
    hkEnum<BodyQualityType, unsigned char> m_qualityType;          /* 12  hkdShape::BodyQualityType */
    unsigned char m_flags;                                         /* 13  */
    unsigned char _pad0E[2];                                       /* 14  (alignment) */
    const struct hkdBreakableShape *m_parent;                      /* 16  */
    hkArray<hkdShapeInstanceInfo> m_children;                      /* 20  */
    hkArray<Connection> m_connections;                             /* 32  hkArray<hkdBreakableShape::Connection> */
    hkRefPtr<const hkpShape> m_physicsShape;                       /* 44  */
    hkRefPtr<const hkdGeometry> m_geometry;                        /* 48  */
    hkRefPtr<const hkdGraphicsShape> m_graphicsShape;              /* 52  */
    const char *m_graphicsShapeName;                               /* 56  */
    hkdProperties m_properties;                                    /* 60  */
    hkArray<hkdAction *> m_actions;                                /* 72  */
    hkRefPtr<hkdFracture> m_dynamicFracture;                       /* 84  */
    float m_strength;                                              /* 88  */
    float m_relativeSubpieceStrength;                              /* 92  */
    float m_tensionLength;                                         /* 96  */
    float m_referenceShapeVolume;                                  /* 100 */
    float m_minDestructionRadius;                                  /* 104 */
    float m_breakingPropagationRate;                               /* 108 */
    unsigned int m_userData;                                       /* 112 */
    unsigned char _pad74[12];                                      /* 116..127 (unmodelled/alignment) */
    hkpMassProperties m_massProps;                                 /* 128 (size 80) -> 208 */
};
typedef struct hkdBreakableShape hkdBreakableShape;
