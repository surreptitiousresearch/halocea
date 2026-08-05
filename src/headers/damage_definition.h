#pragma once
#include <stdint.h>
/* damage_definition — the damage block embedded in a damage_effect (jpt!) tag at tag offset +452.
 * Layout from the database (220 bytes). */

typedef struct damage_definition
{
    int16_t      side_effect;                       /* 0x00 — 2 = harmless/lulling */
    int16_t      category;                           /* 0x02 */
    unsigned int flags;                              /* 0x04 — 0x20 = does-not-hurt-owner-team, 0x40 = skip body,
                                                      *        0x200 = skip shield */
    float        area_of_effect_core_radius;         /* 0x08 */
    float        damage_minimum;                     /* 0x0C */
    float        damage_lower_bound;                 /* 0x10 */
    float        damage_upper_bound;                 /* 0x14 */
    float        vehicle_passthrough_penalty;        /* 0x18 */
    float        active_camouflage_damage;           /* 0x1C */
    float        stun;                               /* 0x20 */
    float        maximum_stun;                       /* 0x24 */
    float        stun_time;                          /* 0x28 */
    unsigned int stun_unused[1];                     /* 0x2C */
    float        instantaneous_acceleration;         /* 0x30 */
    float        instantaneous_acceleration_zero_scale_factor; /* 0x34 */
    unsigned int instantaneous_acceleration_unused[1]; /* 0x38 */
    float        material_modifiers[40];             /* 0x3C */
} damage_definition;                                 /* 0xDC (220 bytes) */
