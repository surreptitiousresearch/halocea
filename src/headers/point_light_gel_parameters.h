#pragma once
/* point_light_gel_parameters — the projected-gel (cookie) block of a point_light_definition (0x48 bytes).
 * A primary and a secondary gel map plus their animation functions. */

#include <stdint.h>
#include "tag_reference.h"

typedef struct point_light_gel_parameters
{
    tag_reference    map;                        /* 0x00 */
    uint16_t pad0;                       /* 0x10 */
    int16_t          texture_animation_function; /* 0x12 */
    float            texture_animation_rate;     /* 0x14 */
    tag_reference    secondary_map;              /* 0x18 */
    uint16_t pad1;                       /* 0x28 */
    int16_t          yaw_function;               /* 0x2A */
    float            yaw_period;                 /* 0x2C */
    uint16_t pad2;                       /* 0x30 */
    int16_t          roll_function;              /* 0x32 */
    float            roll_period;                /* 0x34 */
    uint16_t pad3;                       /* 0x38 */
    int16_t          pitch_function;             /* 0x3A */
    float            pitch_period;               /* 0x3C */
    int              unused[2];                   /* 0x40 */
} point_light_gel_parameters;                     /* 0x48 */
