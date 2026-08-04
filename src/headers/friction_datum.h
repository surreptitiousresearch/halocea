#pragma once
/* friction_datum — decomposition of a friction/force vector into parallel and perpendicular components
 * relative to a surface normal (used by friction_evaluate). DB layout. */

#include "real_vector3d.h"

typedef struct friction_datum
{
    real_vector3d friction;       /* 0x00 — combined result */
    real_vector3d parallel;       /* 0x0C — component parallel to the normal */
    real_vector3d perpendicular;  /* 0x18 — component perpendicular to the normal */
} friction_datum;                 /* 0x24 (36 bytes) */
