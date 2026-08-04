#pragma once
/* game_globals_falling_damage — one element of game_globals_tag.falling_damage (152 bytes). Only the
 * damage-effect tag references reached by reconstructed callers are named; layout from the database. */

#include "tag_reference.h"

typedef struct game_globals_falling_damage
{
    int           falling_unused[2];              /* 0x00 */
    float         falling_distance_lower_bound;    /* 0x08 */
    float         falling_distance_upper_bound;    /* 0x0C */
    tag_reference falling_damage;                  /* 0x10 */
    int           terminal_velocity_unused[2];      /* 0x20 */
    float         maximum_distance;                 /* 0x28 */
    tag_reference maximum_distance_damage;           /* 0x2C */
    tag_reference vehicle_hit_environment_damage_effect; /* 0x3C */
    tag_reference vehicle_killed_unit_damage_effect;     /* 0x4C */
    tag_reference vehicle_collision_damage;              /* 0x5C */
    tag_reference flaming_death_damage;                  /* 0x6C */
    int           unused2[4];                            /* 0x7C */
    float         runtime_maximum_falling_velocity;       /* 0x8C */
    float         runtime_minimum_damage_velocity;        /* 0x90 */
    float         runtime_maximum_damage_velocity;        /* 0x94 */
} game_globals_falling_damage;                            /* 0x98 (152 bytes) */
