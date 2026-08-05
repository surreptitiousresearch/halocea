#pragma once
/* observer_command — a fully-resolved camera viewpoint request produced by a camera proc and consumed
 * by the observer (observer.c). The focus/forward/up block is overlaid with a flat parameters[14]
 * array; the per-parameter flag bytes and timers track in-progress interpolations of each parameter.
 * DB anonymous aggregates reproduced verbatim ($ in identifiers is an MSVC extension). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"

/* DB struct _0C20B91EA6FFBA33097F64548FCDEA17 (56 bytes) — camera-state parameter block; shared by
 * observer_command.___u1 and observer.___u6. */
typedef struct _0C20B91EA6FFBA33097F64548FCDEA17
{
    real_point3d  focus_position; /* 0x00 */
    real_vector3d focus_offset;   /* 0x0C */
    float         focus_distance; /* 0x18 */
    float         field_of_view;  /* 0x1C */
    real_vector3d forward;        /* 0x20 */
    real_vector3d up;             /* 0x2C */
} _0C20B91EA6FFBA33097F64548FCDEA17;

/* DB union _96FA3D8D0396145E4B1FA76BA3C8BDEE (56 bytes) — observer_command member ___u1. */
typedef union _96FA3D8D0396145E4B1FA76BA3C8BDEE
{
    struct _0C20B91EA6FFBA33097F64548FCDEA17 __s0; /* 0x00 */
    float parameters[14];                          /* 0x00 — flat overlay */
} _96FA3D8D0396145E4B1FA76BA3C8BDEE;

/* DB struct _230D652E0E0CA22B04AE94E5CE1D68F0 (5 bytes) — per-parameter flag bytes view. */
typedef struct _230D652E0E0CA22B04AE94E5CE1D68F0
{
    uint8_t position_flags;      /* 0x00 */
    uint8_t focus_offset_flags;  /* 0x01 */
    uint8_t distance_flags;      /* 0x02 */
    uint8_t field_of_view_flags; /* 0x03 */
    uint8_t orientation_flags;   /* 0x04 */
} _230D652E0E0CA22B04AE94E5CE1D68F0;

/* DB union _535E206AE24421D92381F969DB501938 (5 bytes) — observer_command member ___u4. */
typedef union _535E206AE24421D92381F969DB501938
{
    struct _230D652E0E0CA22B04AE94E5CE1D68F0 __s0; /* 0x00 */
    uint8_t parameter_flags[5];            /* 0x00 — flat overlay */
} _535E206AE24421D92381F969DB501938;

/* DB struct _F832A41915FBA18F56360CBB3FCE07BC (20 bytes) — per-parameter timers view. */
typedef struct _F832A41915FBA18F56360CBB3FCE07BC
{
    float position_timer;      /* 0x00 */
    float focus_offset_timer;  /* 0x04 */
    float distance_timer;      /* 0x08 */
    float field_of_view_timer; /* 0x0C */
    float orientation_timer;   /* 0x10 */
} _F832A41915FBA18F56360CBB3FCE07BC;

/* DB union _1E2F38C674286804763D546055946725 (20 bytes) — observer_command member ___u5. */
typedef union _1E2F38C674286804763D546055946725
{
    struct _F832A41915FBA18F56360CBB3FCE07BC __s0; /* 0x00 */
    float parameter_timers[5];                     /* 0x00 — flat overlay */
} _1E2F38C674286804763D546055946725;

typedef struct observer_command
{
    int flags;                                     /* 0x00 — bit0 = command valid */
    /* 0x04 — camera-state parameters. Anonymous wrapper keeps the named ___u1 path and adds
     * direct field/parameters[] aliases (same 56-byte _0C20B91... layout). */
    union {
        union _96FA3D8D0396145E4B1FA76BA3C8BDEE ___u1;
        struct { real_point3d focus_position; real_vector3d focus_offset; float focus_distance; float field_of_view; real_vector3d forward; real_vector3d up; };
        float parameters[14];
    };
    real_vector3d focus_velocity;                  /* 0x3C */
    float         timer;                           /* 0x48 */
    union _535E206AE24421D92381F969DB501938 ___u4; /* 0x4C — per-parameter "snap" flag bytes */
    unsigned char _pad51[3];                       /* 0x51 — db-verified padding */
    /* 0x54 — per-parameter interpolation timers. Anonymous wrapper keeps ___u5 and adds
     * direct parameter_timers[]/named-timer aliases (same 20-byte layout). */
    union {
        union _1E2F38C674286804763D546055946725 ___u5;
        struct { float position_timer; float focus_offset_timer; float distance_timer; float field_of_view_timer; float orientation_timer; };
        float parameter_timers[5];
    };
} observer_command;                                /* 104 bytes */
