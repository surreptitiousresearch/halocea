#pragma once
#include <stdint.h>
/* camera_impulse_definition — a directional camera kick (a transient "temporary" impulse that decays plus a
 * sustained "permanent" lean). Layout from the database (52 bytes). */

typedef struct camera_impulse_definition
{
    float            temporary_duration;            /* 0x00 */
    int16_t          temporary_transition;          /* 0x04 */
    uint16_t pad;                           /* 0x06 */
    float            temporary_rotation;            /* 0x08 */
    float            temporary_translation;         /* 0x0C */
    float            temporary_jitter_lower_bound;  /* 0x10 */
    float            temporary_jitter_upper_bound;  /* 0x14 */
    float            temporary_zero_scale_factor;   /* 0x18 */
    unsigned int     temporary_unused[1];           /* 0x1C */
    float            permanent_angle;               /* 0x20 */
    float            permanent_zero_scale_factor;   /* 0x24 */
    unsigned int     permanent_unused[3];           /* 0x28 */
} camera_impulse_definition;                        /* 0x34 (52 bytes) */
