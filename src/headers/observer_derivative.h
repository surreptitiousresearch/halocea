#pragma once
/* observer_derivative — the 11 interpolated camera scalars plus a rotation offset, used as the
 * position/velocity/acceleration/displacement state of the observer integrator (observer.c).
 * n[0..2] focus position, n[3..5] focus offset, n[6] focus distance, n[7] field of view,
 * n[8..10] orientation (forward/up encoded). DB anonymous aggregates reproduced verbatim
 * ($ in identifiers is an MSVC extension). */

#include "real_vector3d.h"

/* DB struct _F1E76754CE610F8B367410622F8DB9EF (44 bytes) — named scalar view. */
typedef struct _F1E76754CE610F8B367410622F8DB9EF
{
    real_vector3d focus_position; /* 0x00 */
    real_vector3d focus_offset;   /* 0x0C */
    float         focus_distance; /* 0x18 */
    float         field_of_view;  /* 0x1C */
    real_vector3d rotation;       /* 0x20 */
} _F1E76754CE610F8B367410622F8DB9EF;

/* DB union _99E18263404AB9E65F5F8E1713BF3840 (44 bytes) — observer_derivative member ___u0. */
typedef union _99E18263404AB9E65F5F8E1713BF3840
{
    struct _F1E76754CE610F8B367410622F8DB9EF __s0; /* 0x00 */
    float n[11];                                   /* 0x00 — flat scalar view */
} _99E18263404AB9E65F5F8E1713BF3840;

typedef struct observer_derivative
{
    /* 0x00 — scalar block. Anonymous wrapper keeps ___u0 and adds direct field/n[] aliases
     * (same 44-byte _F1E76754... layout). */
    union {
        union _99E18263404AB9E65F5F8E1713BF3840 ___u0;
        struct { real_vector3d focus_position; real_vector3d focus_offset; float focus_distance; float field_of_view; real_vector3d rotation; };
        float n[11];
    };
    real_vector3d rotation_offset;                 /* 0x2C */
} observer_derivative;                             /* 56 bytes */
