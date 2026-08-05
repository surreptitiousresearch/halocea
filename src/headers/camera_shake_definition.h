#pragma once
#include <stdint.h>
/* camera_shake_definition — parameters of a transient camera shake/jitter effect (72 bytes). */

typedef struct camera_shake_definition
{
    float            duration;                     /* 0x00 */
    int16_t          falloff_transition_function;  /* 0x04 */
    uint16_t pad0;                         /* 0x06 */
    float            random_translation_magnitude; /* 0x08 */
    float            random_rotation_magnitude;    /* 0x0C */
    int              unused_motion_scales[3];      /* 0x10 */
    int16_t          periodic_function;            /* 0x1C */
    uint16_t pad1;                          /* 0x1E */
    float            periodic_period;              /* 0x20 */
    float            periodic_weight;              /* 0x24 */
    float            zero_scale_factor;            /* 0x28 */
    int              unused[7];                    /* 0x2C */
} camera_shake_definition;                         /* 0x48 (72 bytes) */
