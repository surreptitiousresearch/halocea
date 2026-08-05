#pragma once
/* actor_ranged_combat_properties — an actor_variant_definition's ranged-combat tuning (284 bytes,
 * DB-verified). Nested burst-geometry/firing-pattern blocks aren't needed by any reconstructed function yet
 * and are left as opaque, correctly-sized placeholders. */

#include <stdint.h>
#include "tag_reference.h"
#include "real_vector3d.h"
#include "actor_burst_geometry.h"
#include "actor_firing_pattern.h"

typedef struct actor_ranged_combat_properties
{
    tag_reference  reference;                          /* 0x00 */
    float          maximum_firing_range;                /* 0x10 */
    float          rate_of_fire;                         /* 0x14 */
    float          projectile_error_angle;               /* 0x18 */
    float          first_burst_delay_lower_bound;        /* 0x1C */
    float          first_burst_delay_upper_bound;        /* 0x20 */
    float          new_target_pattern_time;              /* 0x24 */
    float          surprise_delay_time;                  /* 0x28 */
    float          surprise_wildfire_time;                /* 0x2C */
    float          death_wildfire_chance;                 /* 0x30 */
    float          death_wildfire_time;                   /* 0x34 */
    float          combat_range_lower_bound;               /* 0x38 */
    float          combat_range_upper_bound;               /* 0x3C */
    real_vector3d  gun_offset_stand;                       /* 0x40 */
    real_vector3d  gun_offset_crouch;                      /* 0x4C */
    float          target_track_fraction;                  /* 0x58 */
    float          target_lead_fraction;                   /* 0x5C */
    float          weapon_damage_modifier;                 /* 0x60 */
    float          damage_per_second;                       /* 0x64 */
    actor_burst_geometry burst_geometry;                    /* 0x68 */
    float          special_damage_modifier;                 /* 0x94 */
    float          special_projectile_error;                /* 0x98 */
    actor_firing_pattern new_target_pattern;                /* 0x9C */
    actor_firing_pattern moving_pattern;                     /* 0xB4 */
    actor_firing_pattern berserk_pattern;                    /* 0xCC */
    float          weapon_super_ballistic_range;             /* 0xE4 */
    float          weapon_bombardment_range;                 /* 0xE8 */
    float          modified_vision_range;                    /* 0xEC */
    int16_t        special_fire_mode;                        /* 0xF0 */
    int16_t        special_fire_situation;                   /* 0xF2 */
    float          special_fire_chance;                      /* 0xF4 */
    float          special_fire_delay;                       /* 0xF8 */
    float          melee_range;                              /* 0xFC */
    float          melee_abort_range;                        /* 0x100 */
    float          berserk_firing_range_lower_bound;         /* 0x104 */
    float          berserk_firing_range_upper_bound;         /* 0x108 */
    float          berserk_melee_range;                      /* 0x10C */
    float          berserk_melee_abort_range;                /* 0x110 */
    unsigned int   unused3[2];                               /* 0x114 */
} actor_ranged_combat_properties;                            /* 0x11C = 284 bytes */
