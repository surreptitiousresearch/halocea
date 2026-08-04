#pragma once
#include "hkpWorldObject.h"
#include "hkArray.h"

/* hkpPhantom (160 bytes, DB types_members) — hkpWorldObject-derived broadphase
   phantom. Referenced as an owner pointer / return cast target; body expanded
   from the DB. */

typedef struct hkpPhantomOverlapListener hkpPhantomOverlapListener; /* boundary */
typedef struct hkpPhantomListener hkpPhantomListener;               /* boundary */

struct hkpPhantom : hkpWorldObject
{
    hkArray<hkpPhantomOverlapListener *> m_overlapListeners; /* 0x88 */
    hkArray<hkpPhantomListener *>        m_phantomListeners; /* 0x94 */
};
typedef struct hkpPhantom hkpPhantom;
