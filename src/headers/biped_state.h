#pragma once
/* biped_state — networked snapshot of a biped unit's physics/animation state, saved into a player_update for
 * client-side prediction and replayed during reconciliation. Layout from the database (212 bytes). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"
#include "object_animation_state.h"
#include "unit_animation.h"
#include "real_plane3d.h"

typedef struct biped_state
{
    int                    parent_index;               /* 0x00 */
    real_point3d           position;                   /* 0x04 */
    real_vector3d          translational_velocity;     /* 0x10 */
    real_vector3d          forward;                    /* 0x1C */
    object_animation_state object_animation;           /* 0x28 */
    unit_animation         unit_animation;             /* 0x34 */
    real_point3d           seat_last_position;         /* 0x7C */
    real_vector3d          seat_last_velocity;         /* 0x88 */
    real_vector3d          seat_acceleration;          /* 0x94 */
    real_vector3d          seat_desired_acceleration;  /* 0xA0 */
    unsigned int           biped_flags;                /* 0xAC */
    char                   stop_ticks;                 /* 0xB0 */
    char                   airborne_ticks;             /* 0xB1 */
    char                   slipping_ticks;             /* 0xB2 */
    char                   jump_recovery_timer;        /* 0xB3 */
    int16_t                landing;                    /* 0xB4 */
    unsigned char _pad0[2]; /* db-verified padding */
    float                  crouch;                     /* 0xB8 */
    real_plane3d           ground_plane;               /* 0xBC */
    char                   landing_recovery_counter;   /* 0xCC */
    char                   landing_recovery_time;      /* 0xCD */
    char                   state;                      /* 0xCE */
    unsigned char _pad1[1]; /* db-verified padding */
    int                    support_surface_index;      /* 0xD0 */
} biped_state;                                          /* 212 bytes */
