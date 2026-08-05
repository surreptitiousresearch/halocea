#pragma once
/* _vehicle_definition — the vehicle-level block of the 'vehi' tag (DB types_members-confirmed, 256
 * bytes). Split out of vehicle_definition.h so the composite and its embedded vehicle level are each
 * defined exactly once. The AI/movement fields resolve the doc citations: ai_destination_radius (abs
 * 900), ai_pathfinding_radius (abs 908), ai_charge_repeat_time (abs 912), ai_strafing_stop_range (abs
 * 916), and the oversteer/steering-angle floats (abs 920/924/928). */

#include <stdint.h>
#include "physics_variable_speed.h"
#include "physics_variable_position.h"
#include "tag_reference.h"

typedef struct _vehicle_definition
{
    unsigned int              flags;                          /* 0x00 */
    int16_t                   type;                           /* 0x04 */
    int16_t                   pad;                            /* 0x06 */
    physics_variable_speed    speed;                          /* 0x08 */
    physics_variable_position turn;                           /* 0x18 */
    float                     wheel_circumference;            /* 0x20 */
    float                     turn_rate;                      /* 0x24 */
    float                     blur_speed;                     /* 0x28 */
    int16_t                   function_modes[4];              /* 0x2C */
    float                     unused0[3];                     /* 0x34 */
    physics_variable_speed    slide;                          /* 0x40 */
    float                     flipping_angular_velocity_min;  /* 0x50 */
    float                     flipping_angular_velocity_max;  /* 0x54 */
    float                     unused1[6];                     /* 0x58 */
    float                     fixed_gun_yaw;                  /* 0x70 */
    float                     fixed_gun_pitch;                /* 0x74 */
    float                     unused2[6];                     /* 0x78 */
    float                     ai_sideslip_distance;           /* 0x90 */
    float                     ai_destination_radius;          /* 0x94 (abs 900) */
    float                     ai_avoidance_distance;          /* 0x98 */
    float                     ai_pathfinding_radius;          /* 0x9C (abs 908) */
    float                     ai_charge_repeat_time;          /* 0xA0 (abs 912) */
    float                     ai_strafing_stop_range;         /* 0xA4 (abs 916) */
    float                     ai_oversteer_angle_lower_bound; /* 0xA8 (abs 920) */
    float                     ai_oversteer_angle_upper_bound; /* 0xAC (abs 924) */
    float                     ai_steering_max_angle;          /* 0xB0 (abs 928) */
    float                     ai_steering_max_throttle;       /* 0xB4 (abs 932) */
    float                     ai_movement_max_time;           /* 0xB8 (abs 936) */
    float                     ai_unused;                      /* 0xBC */
    tag_reference             suspension_sound;               /* 0xC0 */
    tag_reference             crash_sound;                    /* 0xD0 */
    tag_reference             material_effects;               /* 0xE0 */
    tag_reference             effect;                         /* 0xF0 */
} _vehicle_definition; /* 256 bytes */
