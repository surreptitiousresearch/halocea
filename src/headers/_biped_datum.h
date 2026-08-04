#pragma once
/* _biped_datum — the biped-class extension of a biped object datum (the tail of the full
 * biped_datum record). DB types_members-confirmed, 132 bytes (0x84). */

#include "real_point3d.h"
#include "real_plane3d.h"
#include "biped_datum_network_data.h"

typedef struct _biped_datum
{
    unsigned int              flags;                                   /* 0x00 */
    char                       landing_recovery_counter;                /* 0x04 */
    char                       landing_recovery_time;                   /* 0x05 */
    char                       state;                                    /* 0x06 */
    char                       elevator_ticks;                           /* 0x07 */
    int                        elevator_object_index;                    /* 0x08 */
    int                        support_surface_index;                    /* 0x0C */
    int                        pathfinding_surface_index;                /* 0x10 */
    real_point3d               pathfinding_point;                        /* 0x14 */
    int                        last_pathfinding_attempt_time;            /* 0x20 */
    int                        last_pathfinding_surface_index;           /* 0x24 */
    int                        impact_target_object_index;               /* 0x28 */
    int                        last_falling_communication_time;          /* 0x2C */
    int                        bump_object_index;                        /* 0x30 */
    char                       bump_ticks;                               /* 0x34 */
    char                       airborne_ticks;                           /* 0x35 */
    char                       slipping_ticks;                           /* 0x36 */
    char                       stop_ticks;                               /* 0x37 */
    char                       jump_recovery_timer;                      /* 0x38 */
    char                       player_melee_ticks;                       /* 0x39 */
    char                       player_melee_attack_tick;                 /* 0x3A */
    char                       _pad3B;                                   /* 0x3B */
    __int16                    landing;                                  /* 0x3C */
    char                       _pad3E[2];                                /* 0x3E */
    float                      crouch;                                    /* 0x40 */
    float                      bank;                                      /* 0x44 */
    real_plane3d               ground_plane;                              /* 0x48 */
    unsigned __int8            limp_body_current_relaxation_iterations;   /* 0x58 */
    unsigned __int8            limp_body_max_relaxation_iterations;       /* 0x59 */
    unsigned __int8            baseline_valid;                            /* 0x5A */
    unsigned __int8            baseline_index;                            /* 0x5B */
    unsigned __int8            message_index;                             /* 0x5C */
    char                       _pad5D[3];                                 /* 0x5D */
    biped_datum_network_data   baseline;                                  /* 0x60 */
    unsigned __int8            last_network_data_valid;                   /* 0x70 */
    char                       _pad71[3];                                 /* 0x71 */
    biped_datum_network_data   last_network_data;                        /* 0x74 */
} _biped_datum;                                                          /* 132 bytes */
