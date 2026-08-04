#pragma once

/* hkpCollidable and its sub-objects, reproduced from the database layout.
   hkpShape / hkAabbUint32 are only referenced through pointers here, so they
   stay forward-declared. */
typedef struct hkpShape hkpShape;
typedef struct hkAabbUint32 hkAabbUint32;

/* offset 0, size 16 */
typedef struct hkpCdBody
{
    const hkpShape *m_shape;
    unsigned int m_shapeKey;
    const void *m_motion;
    const struct hkpCdBody *m_parent;
} hkpCdBody;

/* offset 0, size 4 */
typedef struct hkpBroadPhaseHandle
{
    unsigned int m_id;
} hkpBroadPhaseHandle;

/* size 12 */
typedef struct hkpTypedBroadPhaseHandle
{
    hkpBroadPhaseHandle base;       /* offset 0 (unnamed base) */
    char m_type;                    /* offset 4 */
    char m_ownerOffset;             /* offset 5 */
    char m_objectQualityType;       /* offset 6 */
    unsigned char _pad07;           /* offset 7 */
    unsigned int m_collisionFilterInfo; /* offset 8 */
} hkpTypedBroadPhaseHandle;

/* hkpCollidable::BoundingVolumeData, size 44 */
typedef struct hkpCollidable_BoundingVolumeData
{
    unsigned int m_min[3];              /* offset 0 */
    unsigned char m_expansionMin[3];    /* offset 12 */
    unsigned char m_expansionShift;     /* offset 15 */
    unsigned int m_max[3];              /* offset 16 */
    unsigned char m_expansionMax[3];    /* offset 28 */
    unsigned char m_padding;            /* offset 31 */
    unsigned short m_numChildShapeAabbs;    /* offset 32 */
    unsigned short m_capacityChildShapeAabbs; /* offset 34 */
    hkAabbUint32 *m_childShapeAabbs;    /* offset 36 */
    unsigned int *m_childShapeKeys;     /* offset 40 */
} hkpCollidable_BoundingVolumeData;

/* Local alias so member spellings match the DB's nested-qualified type. */
typedef hkpCollidable_BoundingVolumeData BoundingVolumeData;

/* size 80 */
typedef struct hkpCollidable
{
    hkpCdBody base;                     /* offset 0 (unnamed hkpCdBody base) */
    char m_ownerOffset;                 /* offset 16 */
    unsigned char m_forceCollideOntoPpu;/* offset 17 */
    unsigned short m_shapeSizeOnSpu;    /* offset 18 */
    hkpTypedBroadPhaseHandle m_broadPhaseHandle; /* offset 20 */
    BoundingVolumeData m_boundingVolumeData; /* offset 32 */
    float m_allowedPenetrationDepth;    /* offset 76 */
} hkpCollidable;
