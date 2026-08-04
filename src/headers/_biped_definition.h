#pragma once
/* _biped_definition — biped-specific tag definition block (DB types_members-confirmed, 516 bytes). */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct _biped_definition
{
    float        moving_turning_speed;                          /* 0x000 */
    unsigned int flags;                                         /* 0x004 */
    float        stationary_turning_threshold;                  /* 0x008 */
    float        turning_unused[4];                             /* 0x00C */
    __int16      function_modes[4];                             /* 0x01C */
    tag_reference melee_damage;                                 /* 0x024 */
    float        flying_bank_amount;                            /* 0x034 */
    float        flying_bank_apply_time;                        /* 0x038 */
    float        flying_bank_decay_time;                        /* 0x03C */
    float        flying_pitch_amount;                           /* 0x040 */
    float        flying_velocity;                               /* 0x044 */
    float        flying_sidestep_velocity;                      /* 0x048 */
    float        flying_acceleration;                           /* 0x04C */
    float        flying_deceleration;                           /* 0x050 */
    float        flying_angular_velocity;                       /* 0x054 */
    float        flying_angular_acceleration;                   /* 0x058 */
    float        flying_crouch_velocity_modifier;               /* 0x05C */
    float        flying_unused[2];                              /* 0x060 */
    float        maximum_slope_angle;                           /* 0x068 */
    float        downhill_falloff_angle;                        /* 0x06C */
    float        downhill_cutoff_angle;                         /* 0x070 */
    float        downhill_velocity_scale;                       /* 0x074 */
    float        uphill_falloff_angle;                          /* 0x078 */
    float        uphill_cutoff_angle;                           /* 0x07C */
    float        uphill_velocity_scale;                         /* 0x080 */
    float        velocity_unused[6];                            /* 0x084 */
    tag_reference material_effects;                             /* 0x09C */
    float        more_unused[6];                                /* 0x0AC */
    float        jump_velocity;                                 /* 0x0C4 */
    float        jumping_unused[7];                             /* 0x0C8 */
    float        maximum_soft_landing_time;                     /* 0x0E4 */
    float        maximum_hard_landing_time;                     /* 0x0E8 */
    float        minimum_soft_landing_velocity;                 /* 0x0EC */
    float        minimum_hard_landing_velocity;                 /* 0x0F0 */
    float        maximum_hard_landing_velocity;                 /* 0x0F4 */
    float        death_hard_landing_velocity;                   /* 0x0F8 */
    float        landing_unused[5];                             /* 0x0FC */
    float        standing_camera_height;                        /* 0x110 */
    float        crouching_camera_height;                       /* 0x114 */
    float        crouch_transition_time;                        /* 0x118 */
    float        camera_unused[6];                              /* 0x11C */
    float        collision_height_standing;                     /* 0x134 */
    float        collision_height_crouching;                    /* 0x138 */
    float        collision_radius;                              /* 0x13C */
    float        collision_unused[10];                          /* 0x140 */
    float        autoaim_width;                                 /* 0x168 */
    float        unused[27];                                    /* 0x16C */
    float        runtime_cosine_stationary_turning_threshold;   /* 0x1D8 */
    float        runtime_crouch_transition_velocity;            /* 0x1DC */
    float        runtime_minimum_normal_k;                      /* 0x1E0 */
    float        runtime_downhill_k0;                           /* 0x1E4 */
    float        runtime_downhill_k1;                           /* 0x1E8 */
    float        runtime_uphill_k0;                             /* 0x1EC */
    float        runtime_uphill_k1;                             /* 0x1F0 */
    __int16      runtime_pelvis_node_index;                     /* 0x1F4 */
    __int16      runtime_head_node_index;                       /* 0x1F6 */
    tag_block    contact_points;                                /* 0x1F8 */
} _biped_definition;                     /* 516 bytes */
