#pragma once
/* player_action — one player's per-tick control input (player_control / saved films) */

#include "real_euler_angles2d.h"
#include "real_vector2d.h"

typedef struct player_action
{
    unsigned int        control_flags;          /* 0x00 */
    real_euler_angles2d desired_facing;         /* 0x04 */
    real_vector2d       throttle;               /* 0x0C */
    float               primary_trigger;        /* 0x14 */
    __int16             desired_weapon_index;   /* 0x18 */
    __int16             desired_grenade_index;  /* 0x1A */
    __int16             desired_zoom_level;     /* 0x1C */
    __int16             pad;                    /* 0x1E */
} player_action;                                /* 32 bytes */
