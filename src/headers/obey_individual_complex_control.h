#ifndef OBEY_INDIVIDUAL_COMPLEX_CONTROL_H
#define OBEY_INDIVIDUAL_COMPLEX_CONTROL_H

#include <stdint.h>
#include "real_point3d.h"

typedef struct obey_individual_complex_control
{
    uint8_t override_crouch;                 /* 0x00 */
    unsigned char _pad0[1]; /* db-verified padding */
    int16_t         override_movement_type;          /* 0x02 */
    uint8_t destination_valid;               /* 0x04 */
    uint8_t destination_keep_moving;         /* 0x05 */
    unsigned char _pad1[2]; /* db-verified padding */
    real_point3d    destination_point;               /* 0x08 */
    int             destination_surface_index;       /* 0x14 */
    uint8_t destination_facing;              /* 0x18 */
    unsigned char _pad2[3]; /* db-verified padding */
    real_point3d    destination_facing_point;        /* 0x1C */
    uint8_t destination_radius_valid;        /* 0x28 */
    unsigned char _pad3[3]; /* db-verified padding */
    float           destination_radius;              /* 0x2C */
    uint8_t play_action;                     /* 0x30 */
    unsigned char _pad4[1]; /* db-verified padding */
    int16_t         action_animation_impulse;        /* 0x32 */
    int16_t         action_communication_type;       /* 0x34 */
    uint8_t shoot_at_target;                 /* 0x36 */
    unsigned char _pad5[1]; /* db-verified padding */
    real_point3d    shoot_target;                    /* 0x38 */
    float           shoot_burst_length;              /* 0x44 */
    uint8_t grenade_throw_depress_trigger;   /* 0x48 */
    uint8_t grenade_throw_started;           /* 0x49 */
    int16_t         grenade_throw_trajectory_type;   /* 0x4A */
    real_point3d    grenade_target;                  /* 0x4C */
} obey_individual_complex_control;                   /* 0x58 */

#endif /* OBEY_INDIVIDUAL_COMPLEX_CONTROL_H */
