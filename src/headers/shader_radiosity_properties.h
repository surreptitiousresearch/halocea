#pragma once
/* shader_radiosity_properties — common radiosity/lighting block at the head of every _shader (DB, 32 bytes). */

#include <stdint.h>
#include "real_rgb_color.h"

typedef struct shader_radiosity_properties
{
    uint16_t flags;          /* 0x00 */
    int16_t          detail_level;   /* 0x02 */
    float            power;          /* 0x04 */
    real_rgb_color   color;          /* 0x08 — emitted/scattered light color */
    real_rgb_color   tint_color;     /* 0x14 */
} shader_radiosity_properties;        /* 0x20 */
