#pragma once
/* continuous_damage_effect_definition — the 'cdmg' continuous damage effect tag body (512 bytes).
 * Layout from the database (types_members). */

#include "continuous_vibrate_definition.h"
#include "continuous_camera_shake_definition.h"
#include "damage_definition.h"

typedef struct continuous_damage_effect_definition
{
    float                               falloff_radius;        /* 0x000 */
    float                               cutoff_radius;         /* 0x004 */
    float                               cutoff_scale;          /* 0x008 */
    unsigned int                        area_of_effect_unused[6]; /* 0x00C */
    continuous_vibrate_definition       vibrate;               /* 0x024 (36) */
    continuous_camera_shake_definition  camera_shake;          /* 0x044 (68) */
    unsigned int                        unused[40];            /* 0x084 (132) */
    damage_definition                   damage;                /* 0x124 (292) */
} continuous_damage_effect_definition; /* 512 bytes */
