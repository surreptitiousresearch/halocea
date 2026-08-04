#pragma once
/* _device_definition — the device-level payload ('devi' tag, 276 bytes) embedded at offset 0x17C (380)
 * inside composite device-class tag definitions (machine/control/light fixture), after
 * _object_definition. Layout DB-verified against type '_device_definition' (2026-07-14). */

#include "tag_reference.h"

typedef struct _device_definition
{
    unsigned int  flags;                                          /* 0x00 */
    float         power_transition_time;                          /* 0x04 */
    float         power_acceleration_time;                        /* 0x08 */
    float         powered_position_transition_time;               /* 0x0C */
    float         powered_position_acceleration_time;             /* 0x10 */
    float         depowered_position_transition_time;             /* 0x14 */
    float         depowered_position_acceleration_time;           /* 0x18 */
    __int16       function_modes[4];                              /* 0x1C */
    tag_reference positive_start_effect;                          /* 0x24 */
    tag_reference negative_start_effect;                          /* 0x34 */
    tag_reference positive_stop_effect;                           /* 0x44 */
    tag_reference negative_stop_effect;                           /* 0x54 */
    tag_reference depowered_effect;                               /* 0x64 */
    tag_reference repowered_effect;                               /* 0x74 */
    float         delay_time;                                     /* 0x84 */
    unsigned int  delay_unused[2];                                /* 0x88 */
    tag_reference delay_effect;                                   /* 0x90 */
    float         automatic_activation_radius;                    /* 0xA0 */
    unsigned int  unused[21];                                     /* 0xA4 */
    float         runtime_maximum_power_acceleration;             /* 0xF8 */
    float         runtime_maximum_power_velocity;                 /* 0xFC */
    float         runtime_maximum_depowered_position_acceleration; /* 0x100 */
    float         runtime_maximum_depowered_position_velocity;    /* 0x104 */
    float         runtime_maximum_powered_position_acceleration;  /* 0x108 */
    float         runtime_maximum_powered_position_velocity;      /* 0x10C */
    float         runtime_delay_ticks;                            /* 0x110 */
} _device_definition;                                             /* 276 bytes */
