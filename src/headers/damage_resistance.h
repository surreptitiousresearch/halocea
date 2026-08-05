#pragma once
/* damage_resistance — the damage-resistance / vitality block of a collision_model (or unit) tag;
 * holds body/shield vitality, friendly-fire resistance, damage-effect references, and the per-material
 * and per-region damage tables. Layout from the database (600 bytes). */

#include <stdint.h>
#include "tag_block.h"
#include "tag_reference.h"

typedef struct damage_resistance
{
    unsigned int     flags;                            /* 0x00 — 0x1 = takes-area-damage,
                                                        *        0x2 = takes-damage-when-shield-depleted,
                                                        *        0x20 = headshot/special-resistant */
    int16_t          indirect_damage_material_index;   /* 0x04 */
    uint16_t pad2;                              /* 0x06 */
    float            maximum_body_vitality;             /* 0x08 */
    float            body_system_shock;                 /* 0x0C */
    int              body_vitality_unused[6];           /* 0x10 */
    float            body_stun_unused[7];               /* 0x28 */
    float            friendly_damage_resistance;        /* 0x44 */
    int              friendly_damage_unused[2];         /* 0x48 */
    int              body_unused[8];                    /* 0x50 */
    tag_reference    localized_damage_effect;           /* 0x70 */
    float            area_damage_effect_threshold;      /* 0x80 */
    tag_reference    area_damage_effect;                /* 0x84 */
    float            body_damaged_effect_threshold;     /* 0x94 */
    tag_reference    body_damaged_effect;               /* 0x98 */
    tag_reference    body_depleted_effect;              /* 0xA8 */
    float            body_destroyed_threshold;          /* 0xB8 */
    tag_reference    body_destroyed_effect;             /* 0xBC */
    float            maximum_shield_vitality;           /* 0xCC */
    uint16_t pad0;                              /* 0xD0 */
    int16_t          shield_material_type;              /* 0xD2 */
    int              shield_vitality_unused[6];         /* 0xD4 */
    int16_t          shield_failure_function;           /* 0xEC */
    uint16_t pad1;                              /* 0xEE */
    float            shield_failure_threshold;          /* 0xF0 */
    float            maximum_shield_failure;            /* 0xF4 */
    int              shield_failure_unused[4];          /* 0xF8 */
    float            minimum_shield_stun_damage;        /* 0x108 */
    float            shield_stun_time;                  /* 0x10C */
    float            shield_recharge_time;              /* 0x110 */
    float            shield_recharge_unused[4];         /* 0x114 */
    int              shield_unused[24];                 /* 0x124 */
    float            shield_damaged_effect_threshold;   /* 0x184 */
    tag_reference    shield_damaged_effect;             /* 0x188 */
    tag_reference    shield_depleted_effect;            /* 0x198 */
    tag_reference    shield_recharging_effect;          /* 0x1A8 */
    unsigned int     unused2[2];                        /* 0x1B8 */
    float            runtime_shield_recharge_velocity;  /* 0x1C0 */
    unsigned int     unused[28];                        /* 0x1C4 */
    tag_block        materials;                         /* 0x234 — 72-byte damage_material records */
    tag_block        regions;                           /* 0x240 */
    tag_block        modifiers;                         /* 0x24C */
} damage_resistance;                                    /* 0x258 (600 bytes) */
