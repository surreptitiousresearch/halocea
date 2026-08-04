#pragma once
/* actor_defensive_properties — actor_definition.defensive (108 bytes, DB types_members-confirmed layout). */

typedef struct actor_defensive_properties
{
    float             hide_time_lower_bound;               /* 0x00 */
    float             hide_time_upper_bound;                /* 0x04 */
    float             hide_target_not_visible_time;          /* 0x08 */
    float             shield_fraction_hide;                   /* 0x0C */
    float             shield_fraction_emerge_attack;           /* 0x10 */
    float             shield_fraction_emerge_pursue;           /* 0x14 */
    unsigned int      unused[4];                                /* 0x18 */
    __int16           defensive_crouch_type;                    /* 0x28 */
    unsigned __int16  pad;                                        /* 0x2A */
    float             defensive_threshold_attacking;              /* 0x2C */
    float             defensive_threshold_defending;              /* 0x30 */
    float             defensive_crouch_min_stand_time;             /* 0x34 */
    float             defensive_crouch_min_crouch_time;            /* 0x38 */
    float             defending_hide_time_modifier;                /* 0x3C */
    float             evasion_danger_threshold_attacking;           /* 0x40 */
    float             evasion_danger_threshold_defending;            /* 0x44 */
    float             evasion_seek_cover_chance;                     /* 0x48 */
    float             evasion_delay_timer;                            /* 0x4C */
    float             cover_max_distance;                              /* 0x50 */
    float             cover_damage_threshold;                          /* 0x54 */
    float             stalking_discovery_time;                          /* 0x58 */
    float             stalking_max_distance;                            /* 0x5C */
    float             stationary_facing_angle;                          /* 0x60 */
    float             change_facing_stand_time;                         /* 0x64 */
    unsigned int      unused3[1];                                        /* 0x68 */
} actor_defensive_properties; /* 108 bytes */
