#pragma once
#include "hkReferencedObject.h"
#include "hkpCollidable.h"
#include "hkArray.h"

/* hkpWorldObject and its collidable support types (DB types_members). */

typedef struct hkpWorld hkpWorld;         /* havok/hkpWorld.h */
typedef struct hkpProperty hkpProperty;   /* boundary — per-object property record */

#include "hkMultiThreadCheck.h"

/* hkpLinkedCollidable (92 bytes, DB types_members) — collidable + agent entries. */
struct hkpLinkedCollidable : hkpCollidable
{
    struct CollisionEntry;  /* nested opaque agent-entry record; only used by-pointer in hkArray */
    hkArray<CollisionEntry> m_collisionEntries; /* 0x50 */
};
typedef struct hkpLinkedCollidable hkpLinkedCollidable;

/* hkpWorldObject (136 bytes, DB types_members) — base of entities and phantoms. */
struct hkpWorldObject : hkReferencedObject
{
    hkpWorld *m_world;                    /* 0x08 */
    unsigned int m_userData;              /* 0x0C */
    hkpLinkedCollidable m_collidable;     /* 0x10 */
    hkMultiThreadCheck m_multiThreadCheck;/* 0x6C */
    const char *m_name;                   /* 0x74 */
    hkArray<hkpProperty> m_properties;    /* 0x78 */
    hkReferencedObject *m_aiData;         /* 0x84 */
};
typedef struct hkpWorldObject hkpWorldObject;
