#pragma once
/* vector_short_difference_data — a single tick's __int16-quantized yaw/pitch delta (4 bytes), the
 * higher-precision sibling of vector_char_difference_data. */

typedef struct vector_short_difference_data
{
    __int16 delta_yaw;   /* 0x00 */
    __int16 delta_pitch; /* 0x02 */
} vector_short_difference_data;
