#pragma once
/* continuous_camera_shake_definition — sustained camera-shake parameters embedded in a
 * continuous_damage_effect_definition ('cdmg' tag). Layout from the database (64 bytes). */

typedef struct continuous_camera_shake_definition
{
    float            random_translation_magnitude; /* 0x00 */
    float            random_rotation_magnitude;    /* 0x04 */
    int              unused_motion_scales[3];      /* 0x08 */
    __int16          periodic_function;            /* 0x14 */
    unsigned __int16 pad1;                         /* 0x16 */
    float            periodic_period;              /* 0x18 */
    float            periodic_weight;              /* 0x1C */
    float            zero_scale_factor;            /* 0x20 */
    int              unused[7];                    /* 0x24 */
} continuous_camera_shake_definition; /* 64 bytes */
