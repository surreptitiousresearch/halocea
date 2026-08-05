#pragma once
/* guard_state_data — actor "guard" behavior action_data payload (68 bytes, types_members-confirmed layout).
 * NOTE: the DB models 0x28 as an anonymous union _51C563BEC74FBF94B572D30CFD158F24 (member ___u17)
 *   of guard_firing_position_index and a guard-point sub-struct _397CECB3F587054A9442B926804F4052 —
 *   reproduced verbatim below ($ in identifiers is an MSVC extension, matching the
 *   actor_danger_zone.h convention). */

#include <stdint.h>
#include "real_vector3d.h"
#include "real_point3d.h"

/* DB anonymous struct _397CECB3F587054A9442B926804F4052 (20 bytes) — the guard point itself. */
typedef struct _397CECB3F587054A9442B926804F4052
{
    real_point3d position;      /* 0x00 */
    int          surface_index; /* 0x0C */
    float        radius;        /* 0x10 */
} _397CECB3F587054A9442B926804F4052;                              /* 20 bytes */

/* DB anonymous union _51C563BEC74FBF94B572D30CFD158F24 (20 bytes) — guard_state_data member
 * ___u17 at 0x28: firing-position index or explicit guard point, per guard_location_type. */
typedef union _51C563BEC74FBF94B572D30CFD158F24
{
    int16_t                           guard_firing_position_index; /* 0x00 */
    _397CECB3F587054A9442B926804F4052 guard_point;                 /* 0x00 */
} _51C563BEC74FBF94B572D30CFD158F24;

typedef struct guard_state_data
{
    int16_t          wait_ticks;                    /* 0x00 */
    int16_t          look_ticks;                     /* 0x02 */
    uint8_t  path_begun;                      /* 0x04 */
    uint8_t  post_combat;                      /* 0x05 */
    uint8_t  post_combat_vocalized;             /* 0x06 */
    uint8_t  post_combat_shooting;              /* 0x07 */
    uint8_t  cower;                             /* 0x08 */
    uint8_t  cower_panicked;                    /* 0x09 */
    uint8_t  cower_from_retreat;                /* 0x0A */
    uint8_t  _pad0B;                            /* 0x0B */
    int16_t          cower_ticks;                       /* 0x0C */
    uint8_t  find_new_guard_position;           /* 0x0E */
    uint8_t  shout_about_dead_friend;            /* 0x0F */
    int              shout_dead_friend_prop_index;       /* 0x10 */
    uint8_t  has_guard_direction;                /* 0x14 */
    uint8_t  aim_in_guard_direction;              /* 0x15 */
    uint8_t  _pad16[2];                          /* 0x16 */
    real_vector3d    guard_direction;                    /* 0x18 */
    int16_t          guard_location_type;                /* 0x24 */
    uint8_t  _pad26[2];                          /* 0x26 */
    _51C563BEC74FBF94B572D30CFD158F24 ___u17;            /* 0x28 — anonymous union, defined above */
    int              guard_look_prop_index;               /* 0x3C */
    uint8_t  guard_look_until_reached_point;       /* 0x40 */
    uint8_t  _pad41[3];                           /* 0x41 */
} guard_state_data; /* 68 bytes */
