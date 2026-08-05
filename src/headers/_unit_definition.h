#pragma once
/* _unit_definition — shared unit tag definition block (DB types_members-confirmed, 372 bytes). */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_block.h"
#include "unit_camera.h"
#include "real_vector3d.h"

typedef struct _unit_definition
{
    unsigned int  flags;                                    /* 0x000 */
    int16_t       default_team;                             /* 0x004 */
    int16_t       constant_sound;                           /* 0x006 */
    float         child_damage_fraction;                    /* 0x008 */
    tag_reference integrated_light_toggle_effect;           /* 0x00C */
    int16_t       function_modes[4];                        /* 0x01C */
    float         camera_field_of_view;                     /* 0x024 */
    float         camera_impulse_stiffness;                 /* 0x028 */
    unit_camera   camera;                                   /* 0x02C */
    real_vector3d acceleration_scale;                       /* 0x084 */
    float         integrated_light_poweron_time;            /* 0x090 */
    float         integrated_light_poweroff_time;           /* 0x094 */
    float         integrated_light_lifespan;                /* 0x098 */
    float         soft_ping_threshold;                      /* 0x09C */
    float         soft_ping_minimum_interrupt_time;         /* 0x0A0 */
    float         hard_ping_threshold;                      /* 0x0A4 */
    float         hard_ping_minimum_interrupt_time;         /* 0x0A8 */
    float         hard_death_threshold;                     /* 0x0AC */
    float         feign_death_threshold;                    /* 0x0B0 */
    float         feign_death_time;                         /* 0x0B4 */
    float         evade_distance;                           /* 0x0B8 */
    float         dive_distance;                            /* 0x0BC */
    int           movement_unused;                          /* 0x0C0 */
    float         stunned_movement_threshold;               /* 0x0C4 */
    float         feign_death_chance;                       /* 0x0C8 */
    float         feign_death_repeat_chance;                /* 0x0CC */
    tag_reference spawned_actor_variant;                    /* 0x0D0 */
    int16_t       spawn_actor_lower_bound;                  /* 0x0E0 */
    int16_t       spawn_actor_upper_bound;                  /* 0x0E2 */
    float         spawn_throw_velocity;                     /* 0x0E4 */
    float         aiming_velocity_maximum;                  /* 0x0E8 */
    float         aiming_acceleration_maximum;              /* 0x0EC */
    float         casual_aiming_modifier;                   /* 0x0F0 */
    float         looking_velocity_maximum;                 /* 0x0F4 */
    float         looking_acceleration_maximum;             /* 0x0F8 */
    int           unused4[2];                               /* 0x0FC */
    float         ai_vehicle_avoidance_radius;              /* 0x104 */
    float         ai_danger_radius;                         /* 0x108 */
    tag_reference melee_damage;                             /* 0x10C */
    int16_t       blip_type;                                /* 0x11C — motion-sensor blip type; also reused in
                                                             * physics_compute_biped_collision as an index into
                                                             * the `scales` global */
    int16_t       padxxx;                                   /* 0x11E */
    int           unused[3];                                /* 0x120 */
    tag_block     huds;                                     /* 0x12C */
    tag_block     dialogue_variants;                        /* 0x138 */
    float         grenade_velocity;                         /* 0x144 */
    int16_t       grenade_type;                             /* 0x148 */
    int16_t       grenade_count;                            /* 0x14A */
    int16_t       runtime_soft_ping_minimum_interrupt_ticks;/* 0x14C */
    int16_t       runtime_hard_ping_minimum_interrupt_ticks;/* 0x14E */
    tag_block     powered_seats;                            /* 0x150 */
    tag_block     initial_weapons;                          /* 0x15C */
    tag_block     seats;                                    /* 0x168 */
} _unit_definition;              /* 372 bytes */
