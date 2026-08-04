#pragma once
/* _projectile_definition — the projectile-specific definition level of the 'proj' tag,
 * embedded at 0x17C inside the composite projectile_definition. DB layout (208 bytes). */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct _projectile_definition
{
    unsigned int     flags;                          /* 0x00 — 0x8 = combine initial velocity w/ parent (grouped super detonation) */
    __int16          detonation_timer_mode;          /* 0x04 */
    __int16          impact_noise;                   /* 0x06 */
    __int16          function_modes[4];              /* 0x08 */
    tag_reference    super_detonation_effect;        /* 0x10 */
    float            danger_perception_radius;       /* 0x20 */
    float            collision_radius;               /* 0x24 */
    float            arming_time;                    /* 0x28 */
    float            danger_radius;                  /* 0x2C */
    tag_reference    detonation_effect;              /* 0x30 */
    float            detonation_minimum_time;        /* 0x40 */
    float            detonation_maximum_time;        /* 0x44 */
    float            detonation_minimum_velocity;    /* 0x48 */
    float            detonation_maximum_range;       /* 0x4C */
    float            air_gravity_scale;              /* 0x50 */
    float            air_minimum_damage_distance;    /* 0x54 */
    float            air_maximum_damage_distance;    /* 0x58 */
    float            water_gravity_scale;            /* 0x5C */
    float            water_minimum_damage_distance;  /* 0x60 */
    float            water_maximum_damage_distance;  /* 0x64 */
    float            initial_velocity;               /* 0x68 */
    float            final_velocity;                 /* 0x6C */
    float            guided_angular_velocity;        /* 0x70 */
    __int16          detonation_noise;               /* 0x74 */
    unsigned __int16 unused1;                        /* 0x76 */
    tag_reference    detonation_timer_started;       /* 0x78 */
    tag_reference    flyby_sound;                    /* 0x88 */
    tag_reference    detonation_damage;              /* 0x98 */
    tag_reference    impact_damage;                  /* 0xA8 */
    int              unused2[3];                     /* 0xB8 */
    tag_block        material_responses;             /* 0xC4 — projectile_material_response_definition[] */
} _projectile_definition;                            /* 208 bytes */
