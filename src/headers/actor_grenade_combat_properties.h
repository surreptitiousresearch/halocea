#pragma once
/* actor_grenade_combat_properties — actor_variant_definition.grenade_combat (64 bytes), per an
 * authoritative structure dump (C:\halo\headers_ref\actor_grenade_combat_properties.h). Sits
 * immediately after ranged_combat at absolute byte 384 (0x180) — confirmed by 9 of the doc's raw
 * "variant +0xNN"/"+NNN" grenade citations all landing exactly on named fields. */

typedef struct actor_grenade_combat_properties
{
    __int16  grenade_type;                  /* 0x00 (abs 384) */
    __int16  trajectory_type;                /* 0x02 (abs 386) */
    __int16  stimulus_type;                   /* 0x04 (abs 388) */
    __int16  minimum_enemy_count;              /* 0x06 (abs 390) — doc's "minimum_enemy_count (+0x186)" */
    float    enemy_radius;                       /* 0x08 (abs 392) — doc's "enemy_radius +0x188" */
    float    grenade_unused;                      /* 0x0C (abs 396) */
    float    grenade_maximum_velocity;              /* 0x10 (abs 400) — doc's "velocity_max (+400)" */
    float    grenade_range_lower_bound;              /* 0x14 (abs 404) — doc's "+404" grenade range lower */
    float    grenade_range_upper_bound;               /* 0x18 (abs 408) — doc's "+408" grenade range upper */
    float    collateral_damage_radius;                 /* 0x1C (abs 412) — doc's "collateral_radius +0x19C" */
    float    throw_grenade_chance;                       /* 0x20 (abs 416) — doc's "grenade_throw_chance (+416)" */
    float    throw_grenade_delay;                         /* 0x24 (abs 420) — doc's "grenade_throw_interval (+420)" */
    float    encounter_grenade_timeout;                     /* 0x28 (abs 424) — doc's "variant_grenade_chance(+0x1A8)" */
    unsigned int unused2[5];                                 /* 0x2C */
} actor_grenade_combat_properties; /* 0x40 = 64 bytes */
