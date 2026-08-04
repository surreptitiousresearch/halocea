#pragma once
#include "hkReferencedObject.h"
#include "hkpPhantom.h"

typedef struct hkpWorld hkpWorld;
typedef struct hkpWorldDeletionListener_vtbl hkpWorldDeletionListener_vtbl;
typedef struct hkpPhantomOverlapListener_vtbl hkpPhantomOverlapListener_vtbl;

/* types_members hkpWorldDeletionListener (size 4) */
typedef struct hkpWorldDeletionListener
{
    hkpWorldDeletionListener_vtbl *__vftable; /* 0 */
} hkpWorldDeletionListener;

/* types_members hkpPhantomOverlapListener (size 4) */
typedef struct hkpPhantomOverlapListener
{
    hkpPhantomOverlapListener_vtbl *__vftable; /* 0 */
} hkpPhantomOverlapListener;

/* types_enum_values hkpWorldCinfo::BroadPhaseBorderBehaviour */
enum BroadPhaseBorderBehaviour
{
    BROADPHASE_BORDER_ASSERT = 0,
    BROADPHASE_BORDER_FIX_ENTITY = 1,
    BROADPHASE_BORDER_REMOVE_ENTITY = 2,
    BROADPHASE_BORDER_DO_NOTHING = 3,
};

/* size 48 — triple-inherited: hkReferencedObject + the two listener interfaces.
   Layout from DB types_members hkpBroadPhaseBorder. */
struct hkpBroadPhaseBorder : hkReferencedObject, hkpWorldDeletionListener, hkpPhantomOverlapListener
{
    hkpWorld *m_world;                 /* 16 */
    hkpPhantom *m_phantoms[6];         /* 20 the six AABB-border phantoms */
    BroadPhaseBorderBehaviour m_type;  /* 44 hkpWorldCinfo::BroadPhaseBorderBehaviour */
};
typedef struct hkpBroadPhaseBorder hkpBroadPhaseBorder;
