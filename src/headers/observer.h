#pragma once
/* observer — one local player's camera/observer record (0x29C = 668 bytes). Holds the active command state,
 * the resolved viewpoint (result), the current camera-state block (focus/orientation, overlaid with a flat
 * positions[14] array), and the integrator state (velocities/accelerations/displacements plus the per-scalar
 * interpolation coefficient rows a..f). observer_globals holds a 4-byte header followed by the per-local-player
 * observer records at stride 0x29C. */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "observer_command.h"
#include "observer_result.h"
#include "observer_derivative.h"

/* DB union _1EF137778B63593FC7AD4C8A5C81CC01 (56 bytes) — observer member ___u6; shares the
 * _0C20B91EA6FFBA33097F64548FCDEA17 camera-state block defined in observer_command.h
 * ($ in identifiers is an MSVC extension). */
typedef union _1EF137778B63593FC7AD4C8A5C81CC01
{
    struct _0C20B91EA6FFBA33097F64548FCDEA17 __s0; /* 0x00 */
    float positions[14];                           /* 0x00 — flat overlay */
} _1EF137778B63593FC7AD4C8A5C81CC01;

typedef struct observer
{
    int               header_signature;    /* 0x000 — 'obsr' (1918985249) */
    observer_command *pending_command;     /* 0x004 */
    observer_command  last_command;        /* 0x008 */
    unsigned __int8   updated_for_frame;   /* 0x070 */
    unsigned __int8   first_command;       /* 0x071 */
    char              _pad072[2];          /* 0x072 */
    observer_result   result;              /* 0x074 — resolved viewpoint */
    /* 0x0B0 — current camera state. Anonymous wrapper keeps the named ___u6 path and adds
     * direct field/positions[] aliases (same 56-byte layout as _0C20B91...). */
    union {
        union _1EF137778B63593FC7AD4C8A5C81CC01 ___u6;
        struct { real_point3d focus_position; real_vector3d focus_offset; float focus_distance; float field_of_view; real_vector3d forward; real_vector3d up; };
        float positions[14];
    };
    observer_derivative velocities;        /* 0x0E8 */
    observer_derivative accelerations;     /* 0x120 */
    float               a[11];             /* 0x158 — per-scalar interpolation coefficient rows */
    float               b[11];             /* 0x184 */
    float               c[11];             /* 0x1B0 */
    float               d[11];             /* 0x1DC */
    float               e[11];             /* 0x208 */
    float               f[11];             /* 0x234 */
    observer_derivative displacements;     /* 0x260 */
    int                 trailer_signature; /* 0x298 — 'obsr' (1918985249) */
} observer;                                /* 0x29C = 668 bytes */
