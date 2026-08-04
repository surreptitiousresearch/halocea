#pragma once
/* game_globals_player_control — the game globals' player-control tuning block (128 bytes, DB
 * types_members layout). Pointed at by global_game_globals->player_control.address. */

#include "tag_block.h"

typedef struct game_globals_player_control
{
    float     magnetism_friction;                 /* 0x00 */
    float     magnetism_adhesion;                 /* 0x04 */
    float     magnetism_inconsequential_target_scale; /* 0x08 */
    float     magnetism_unused[13];               /* 0x0C */
    float     look_acceleration_time;             /* 0x40 (64) */
    float     look_acceleration_scale;            /* 0x44 (68) */
    float     look_pegging_threshold;             /* 0x48 (72) */
    float     look_default_pitch_rate;            /* 0x4C (76) */
    float     look_default_yaw_rate;              /* 0x50 (80) */
    float     look_autolevel_scale;               /* 0x54 (84) */
    float     look_unused[5];                     /* 0x58 (88) */
    __int16   minimum_weapon_swap_ticks;          /* 0x6C (108) */
    __int16   minimum_autolevel_enabled_ticks;    /* 0x6E (110) */
    float     minimum_vehicle_flipping_angle;     /* 0x70 (112) */
    tag_block look_function;                      /* 0x74 (116) */
} game_globals_player_control;                    /* 128 bytes */
