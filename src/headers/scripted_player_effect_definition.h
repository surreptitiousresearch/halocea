#pragma once
/* scripted_player_effect_definition — a script-driven camera/controller shake applied to the player.
 * Layout from the database (32 bytes). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_euler_angles3d.h"

typedef struct scripted_player_effect_definition
{
    real_point3d        max_translation; /* 0x00 */
    real_euler_angles3d max_rotation;    /* 0x0C */
    float               max_intensity;   /* 0x18 */
    int16_t             timer;           /* 0x1C */
    int16_t             total_time;      /* 0x1E */
} scripted_player_effect_definition;     /* 32 bytes */
