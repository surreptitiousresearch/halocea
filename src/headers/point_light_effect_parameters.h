#pragma once
/* point_light_effect_parameters — the flicker/transition-effect block of a point_light_definition
 * (0x10 bytes). duration divides the elapsed-time-since-transition-start to get a normalized
 * 0..1 progress fed to falloff_function. */

typedef struct point_light_effect_parameters
{
    float             duration;         /* 0x00 */
    unsigned __int16  pad;              /* 0x04 */
    __int16           falloff_function; /* 0x06 */
    float             unused[2];        /* 0x08 */
} point_light_effect_parameters; /* 0x10 */
