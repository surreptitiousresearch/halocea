#pragma once
/* actor_output_data — actor_datum member at 0x6D0 (1744), 84 bytes, running to the end of the struct at
 * 0x724 (1828). Full layout from an authoritative actor_datum structure dump (ground truth; supersedes
 * any prior opaque placeholder). */

#include "actor_animation_orders.h"
#include "real_vector3d.h"

typedef struct actor_output_data
{
    unsigned int             control_flags;              /* 0x00 */
    __int16                  persistent_control_ticks;    /* 0x04 */
    unsigned char            _pad06[2];                   /* 0x06 */
    unsigned int             persistent_control_flags;    /* 0x08 */
    __int16                  movement_type;                /* 0x0C */
    unsigned char            _pad0E[2];                    /* 0x0E */
    real_vector3d            throttle;                     /* 0x10 */
    actor_animation_orders   animation;                    /* 0x1C */
    __int16                  aiming_speed;                 /* 0x28 */
    unsigned char            _pad2A[2];                    /* 0x2A */
    real_vector3d            facing_vector;                /* 0x2C */
    real_vector3d            aiming_vector;                /* 0x38 */
    real_vector3d            looking_vector;               /* 0x44 */
    float                    analog_primary_trigger;       /* 0x50 */
} actor_output_data; /* 0x54 = 84 bytes */
