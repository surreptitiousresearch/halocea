#pragma once
/* screen_flash_definition — parameters of a full-screen color flash effect (56 bytes). */

#include "real_argb_color.h"

typedef struct screen_flash_definition
{
    __int16         type;               /* 0x00 */
    __int16         priority;           /* 0x02 */
    unsigned int    unused1[3];         /* 0x04 */
    float           duration;           /* 0x10 */
    __int16         fade_function;      /* 0x14 */
    unsigned __int16 pad;               /* 0x16 */
    unsigned int    unused2[2];         /* 0x18 */
    float           max_intensity;      /* 0x20 */
    float           zero_scale_factor;  /* 0x24 */
    real_argb_color screen_flash_color; /* 0x28 */
} screen_flash_definition;              /* 0x38 (56 bytes) */
