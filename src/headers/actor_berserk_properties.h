#pragma once
/* actor_berserk_properties — actor_definition.berserk (64 bytes), per an authoritative structure
 * dump (C:\halo\headers_ref\actor_berserk_properties.h). Sits at absolute byte 888 within
 * actor_definition — confirmed by 8 of the doc's raw-offset citations (charge cooldown, point-blank
 * charge distance, leap params, berserk-from-damage health/shield thresholds, berserk-from-proximity
 * distance, and berserk-from-attached-projectiles roll) all landing exactly on named fields. */

typedef struct actor_berserk_properties
{
    float  melee_attack_delay_timer;      /* 0x00 (abs 888) — doc's charge-cooldown "definition_charge_delay" */
    float  melee_attack_range;             /* 0x04 (abs 892) — doc's point-blank-charge allowance distance */
    float  melee_attack_charge_timer;       /* 0x08 (abs 896) */
    float  melee_leap_range_lower_bound;     /* 0x0C (abs 900) — doc's leap_range lower bound */
    float  melee_leap_range_upper_bound;      /* 0x10 (abs 904) — doc's leap_range upper bound */
    float  melee_leap_velocity;                /* 0x14 (abs 908) */
    float  melee_leap_chance;                   /* 0x18 (abs 912) — doc's leap roll chance */
    float  melee_leap_ballistic;                  /* 0x1C (abs 916) */
    float  damage_berserk_amount;                  /* 0x20 (abs 920) — doc's berserk-from-damage health threshold */
    float  damage_berserk_threshold;                /* 0x24 (abs 924) — doc's berserk-from-damage shield threshold */
    float  proximity_berserk_distance;                /* 0x28 (abs 928) — doc's berserk-from-proximity distance */
    float  suicide_sensing_distance;                    /* 0x2C (abs 932) */
    float  grenade_attached_chance;                      /* 0x30 (abs 936) — doc's berserk-from-attached-projectiles roll */
    unsigned int unused_berserk[3];                       /* 0x34 */
} actor_berserk_properties; /* 0x40 = 64 bytes */
