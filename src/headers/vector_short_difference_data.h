#pragma once
#include <stdint.h>
/* vector_short_difference_data — a single tick's __int16-quantized yaw/pitch delta (4 bytes), the
 * higher-precision sibling of vector_char_difference_data. */

typedef struct vector_short_difference_data
{
    int16_t delta_yaw;   /* 0x00 */
    int16_t delta_pitch; /* 0x02 */
} vector_short_difference_data;
