#pragma once
/* _weapon_definition — the weapon-level payload ('weap' tag, 512 bytes) that sits at offset 0x308
 * inside the composite weapon_definition, after the shared _object_definition (0x00) and
 * _item_definition (0x17C) levels. Layout DB-verified against type '_weapon_definition' (2026-07-13). */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_block.h"
#include "aim_assist_parameters.h"
#include "weapon_interface_definition.h"

typedef struct _weapon_definition
{
    unsigned int    flags;                          /* 0x00 */
    char            label[32];                       /* 0x04 */
    int16_t         secondary_trigger_mode;            /* 0x24 */
    int16_t         maximum_alternate_shots_loaded;      /* 0x26 */
    int16_t         function_modes[4];                    /* 0x28 */
    float           ready_time;                             /* 0x30 */
    tag_reference   ready_effect;                             /* 0x34 */
    float           heat_recovery_threshold;                    /* 0x44 */
    float           heat_overheated_threshold;                    /* 0x48 */
    float           heat_detonation_threshold;                      /* 0x4C */
    float           overheated_explosion_fraction;                    /* 0x50 */
    float           heat_loss_per_second;                                /* 0x54 */
    float           heat_illumination;                                    /* 0x58 */
    float           heat_unused[4];                                         /* 0x5C */
    tag_reference   overheated_effect;                                        /* 0x6C */
    tag_reference   detonation_effect;                                          /* 0x7C */
    tag_reference   melee_attack_damage;                                          /* 0x8C */
    tag_reference   melee_attack_response;                                          /* 0x9C */
    unsigned int    unused_sabine_women[2];                                          /* 0xAC */
    tag_reference   ai_firing_parameters;                                              /* 0xB4 */
    float           near_reticle_range;                                                 /* 0xC4 */
    float           far_reticle_range;                                                    /* 0xC8 */
    float           intersection_reticle_range;                                            /* 0xCC */
    uint16_t pad;                                                                    /* 0xD0 */
    int16_t         zoom_level_count;                                                          /* 0xD2 */
    float           zoom_magnification_minimum;                                                  /* 0xD4 */
    float           zoom_magnification_maximum;                                                    /* 0xD8 */
    aim_assist_parameters aim_assist_parameters;                                                     /* 0xDC */
    int16_t         movement_penalty_mode;                                                             /* 0xF4 */
    uint16_t pad1;                                                                               /* 0xF6 */
    float           forward_movement_penalty;                                                              /* 0xF8 */
    float           sideways_movement_penalty;                                                               /* 0xFC */
    float           properties_unused;                                                                         /* 0x100 */
    float           ai_minimum_target_range;                                                                      /* 0x104 */
    float           ai_idle_look_time_modifier;                                                                      /* 0x108 */
    float           ai_unused;                                                                                         /* 0x10C */
    float           integrated_light_poweron_time;                                                                        /* 0x110 */
    float           integrated_light_poweroff_time;                                                                          /* 0x114 */
    tag_reference   integrated_light_on_effect;                                                                                /* 0x118 */
    tag_reference   integrated_light_off_effect;                                                                                 /* 0x128 */
    float           age_heat_recovery_penalty;                                                                                     /* 0x138 */
    float           age_rate_of_fire_penalty;                                                                                         /* 0x13C */
    float           age_misfire_start;                                                                                                  /* 0x140 */
    float           age_misfire_chance;                                                                                                    /* 0x144 */
    float           age_unused[3];                                                                                                            /* 0x148 */
    weapon_interface_definition interface_definition;                                                                                      /* 0x154 */
    tag_reference   pickup_sound;                                                                                                                    /* 0x188 */
    tag_reference   zoom_in_sound;                                                                                                                      /* 0x198 */
    tag_reference   zoom_out_sound;                                                                                                                        /* 0x1A8 */
    tag_block       unused_block;                                                                                                                            /* 0x1B8 */
    float           active_camo_ding;                                                                                                                          /* 0x1C4 */
    float           active_camo_regrowth_rate;                                                                                                                    /* 0x1C8 */
    int             unused4[3];                                                                                                                                     /* 0x1CC */
    int16_t         unused5;                                                                                                                                          /* 0x1D8 */
    int16_t         weapon_type;                                                                                                                                        /* 0x1DA */
    tag_block       predicted_resources;                                                                                                                                  /* 0x1DC */
    tag_block       magazines;                                                                                                                                              /* 0x1E8 */
    tag_block       triggers;                                                                                                                                                 /* 0x1F4 */
} _weapon_definition; /* 0x200 = 512 bytes */
