#pragma once
#include "hkVector4.h"

/* hkQuaternion (size 16, DB types_members) — a unit quaternion stored as a single
   hkVector4 (x,y,z laid in the vector lanes, w in the last lane). */
typedef struct hkQuaternion
{
    hkVector4 m_vec; /* 0x00 */
} hkQuaternion;

#include "hkRotation.h"   /* not a fwd decl: under C hkRotation aliases hkMatrix3, so a struct tag
                             would name a different (incomplete) type than callers pass */

void hkQuaternion_set(hkQuaternion *self, const hkRotation *r); /* hkQuaternion::set */
