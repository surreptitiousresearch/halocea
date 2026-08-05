#pragma once
/* screen_flash_definition — parameters of a full-screen color flash effect (56 bytes). */

#include <stdint.h>
#include "real_argb_color.h"

typedef struct screen_flash_definition
{
    int16_t         type;               /* 0x00 */
    int16_t         priority;           /* 0x02 */
    unsigned int    unused1[3];         /* 0x04 */
    float           duration;           /* 0x10 */
    int16_t         fade_function;      /* 0x14 */
    uint16_t pad;               /* 0x16 */
    unsigned int    unused2[2];         /* 0x18 */
    float           max_intensity;      /* 0x20 */
    float           zero_scale_factor;  /* 0x24 */
    real_argb_color screen_flash_color; /* 0x28 */
} screen_flash_definition;              /* 0x38 (56 bytes) */
