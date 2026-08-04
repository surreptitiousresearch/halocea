#pragma once
/* actor_stimulus_data — actor_datum member at 0x2EC (748), 100 bytes. Full layout from an authoritative
 * actor_datum structure dump (ground truth; supersedes any prior opaque placeholder). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct actor_stimulus_data
{
    unsigned __int8  was_damaged;                                       /* 0x00 */
    unsigned __int8  vehicle_eviction;                                   /* 0x01 */
    __int16          surprise_level;                                     /* 0x02 */
    unsigned __int8  was_surprised;                                      /* 0x04 */
    unsigned char    _pad05[3];                                          /* 0x05 */
    int              surprise_prop_index;                                /* 0x08 */
    unsigned __int8  surprise_has_vector;                                /* 0x0C */
    unsigned char    _pad0D[3];                                          /* 0x0D */
    real_vector3d    surprise_vector;                                    /* 0x10 */
    __int16          panic_type;                                         /* 0x1C */
    unsigned char    _pad1E[2];                                          /* 0x1E */
    int              panic_prop_index;                                   /* 0x20 */
    __int16          berserk_type;                                       /* 0x24 */
    __int16          combat_transition;                                  /* 0x26 */
    unsigned __int8  combat_transition_guard_at_point;                   /* 0x28 */
    unsigned char    _pad29[3];                                          /* 0x29 */
    real_point3d     combat_transition_guard_point;                      /* 0x2C */
    int              combat_transition_guard_point_surface_index;        /* 0x38 */
    float            combat_transition_guard_point_distance;             /* 0x3C */
    unsigned __int8  combat_transition_has_vector;                       /* 0x40 */
    unsigned char    _pad41[3];                                          /* 0x41 */
    real_vector3d    combat_transition_vector;                           /* 0x44 */
    int              combat_transition_guard_timer;                      /* 0x50 */
    int              combat_transition_prop_index;                       /* 0x54 */
    int              combat_transition_prop_look_timer;                  /* 0x58 */
    unsigned __int8  combat_transition_prop_look_while_moving;           /* 0x5C */
    unsigned char    _pad5D;                                             /* 0x5D */
    __int16          suspicion_combat_status;                            /* 0x5E */
    int              suspicion_timer;                                    /* 0x60 */
} actor_stimulus_data; /* 0x64 = 100 bytes */
