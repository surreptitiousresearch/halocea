#pragma once
/* player_control @ player_control — per-player control input state (player_control.c).
 * 64-byte runtime record. */

#include <stdint.h>
#include "real_euler_angles2d.h"
#include "real_vector2d.h"

typedef struct player_control
{
    int              unit_index;                            /* 0x00 */
    unsigned int     control_flags;                         /* 0x04 */
    uint16_t inhibited_button_bit_vector;           /* 0x08 */
    uint16_t reset_button_when_released_bit_vector; /* 0x0A */
    real_euler_angles2d desired_angles;                     /* 0x0C */
    real_vector2d    throttle;                              /* 0x14 */
    float            primary_trigger;                       /* 0x1C */
    int16_t          desired_weapon_index;                  /* 0x20 */
    int16_t          desired_grenade_index;                 /* 0x22 */
    int16_t          desired_zoom_level;                    /* 0x24 */
    uint8_t  use_autolevel;                         /* 0x26 */
    char             autolevel_ticks;                       /* 0x27 */
    int              target_object_index;                   /* 0x28 */
    float            autoaim_level;                         /* 0x2C */
    float            magnetism_level;                       /* 0x30 */
    float            look_acceleration_time;                /* 0x34 */
    float            pitch_minimum;                         /* 0x38 */
    float            pitch_maximum;                         /* 0x3C */
} player_control;                                           /* 64 bytes */
