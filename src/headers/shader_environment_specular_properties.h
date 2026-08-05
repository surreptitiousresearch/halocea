#pragma once
/* shader_environment_specular_properties — perpendicular/parallel specular tint block (DB, 84 bytes). */

#include <stdint.h>
#include "real_rgb_color.h"

typedef struct shader_environment_specular_properties
{
    uint16_t flags;                      /* 0x00 */
    int16_t          type;                       /* 0x02 */
    int              unused1[4];                 /* 0x04 */
    float            brightness;                 /* 0x14 */
    int              unused2[5];                 /* 0x18 */
    real_rgb_color   view_perpendicular_color;   /* 0x2C */
    real_rgb_color   view_parallel_color;        /* 0x38 */
    int              unused3[4];                 /* 0x44 */
} shader_environment_specular_properties;          /* 0x54 */
