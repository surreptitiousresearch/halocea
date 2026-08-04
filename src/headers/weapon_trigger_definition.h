#pragma once
/* weapon_trigger_definition — one element of weapon_definition.weapon.triggers ('weap' tag trigger block),
 * 276 bytes. Full DB types_members layout. charging_time > 0 marks a charge-up trigger (plasma pistol
 * overcharge et al.) — the field AI charge-counter checks read. */

#include "tag_reference.h"
#include "tag_block.h"
#include "real_point3d.h"

typedef struct weapon_trigger_definition
{
    unsigned int  flags;                                    /* 0x000 */
    float         initial_rate_of_fire;                     /* 0x004 */
    float         final_rate_of_fire;                       /* 0x008 */
    float         rate_of_fire_acceleration_time;           /* 0x00C */
    float         rate_of_fire_deceleration_time;           /* 0x010 */
    float         blurred_rate_of_fire;                     /* 0x014 */
    float         rate_of_fire_unused[2];                   /* 0x018 */
    __int16       magazine_index;                           /* 0x020 */
    __int16       rounds_per_shot;                          /* 0x022 */
    __int16       minimum_rounds_loaded_per_shot;           /* 0x024 */
    __int16       rounds_between_tracers;                   /* 0x026 */
    __int16       ammunition_unused[3];                     /* 0x028 */
    __int16       firing_noise;                             /* 0x02E */
    float         initial_error;                            /* 0x030 */
    float         final_error;                              /* 0x034 */
    float         error_acceleration_time;                  /* 0x038 */
    float         error_deceleration_time;                  /* 0x03C */
    float         error_unused[2];                          /* 0x040 */
    float         charging_time;                            /* 0x048 */
    float         charged_time;                             /* 0x04C */
    __int16       overcharged_action;                       /* 0x050 */
    unsigned __int16 pad;                                   /* 0x052 */
    float         charged_illumination;                     /* 0x054 */
    float         spew_time;                                /* 0x058 */
    tag_reference charging_effect;                          /* 0x05C */
    __int16       projectile_distribution_function;         /* 0x06C */
    __int16       projectiles_per_shot;                     /* 0x06E */
    float         projectile_distribution_angle;            /* 0x070 */
    float         projectile_unused0[1];                    /* 0x074 */
    float         projectile_error_inner_cone_angle;        /* 0x078 */
    float         projectile_error_angle_lower_bound;       /* 0x07C */
    float         projectile_error_angle_upper_bound;       /* 0x080 */
    real_point3d  first_person_weapon_offset;               /* 0x084 */
    float         projectile_unused1[1];                    /* 0x090 */
    tag_reference projectile;                               /* 0x094 */
    float         ejection_port_recovery_time;              /* 0x0A4 */
    float         illumination_recovery_time;               /* 0x0A8 */
    float         ejection_port_unused[3];                  /* 0x0AC */
    float         heat_generated_per_round;                 /* 0x0B8 */
    float         age_generated_per_round;                  /* 0x0BC */
    float         heat_unused[1];                           /* 0x0C0 */
    float         overloading_time;                         /* 0x0C4 */
    int           overloading_unused[2];                    /* 0x0C8 */
    int           unused[8];                                /* 0x0D0 */
    float         runtime_illumination_recovery_time;       /* 0x0F0 */
    float         runtime_ejection_port_recovery_time;      /* 0x0F4 */
    float         runtime_rate_of_fire_acceleration_time;   /* 0x0F8 */
    float         runtime_rate_of_fire_deceleration_time;   /* 0x0FC */
    float         runtime_error_acceleration_time;          /* 0x100 */
    float         runtime_error_deceleration_time;          /* 0x104 */
    tag_block     firing_effects;                           /* 0x108 */
} weapon_trigger_definition;                                /* 276 bytes (0x114) */
