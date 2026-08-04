#pragma once
/* vector_char_difference_data — a single tick's byte-quantized yaw/pitch delta, as read off a
 * controller stick and packed for network transport (2 bytes). */

typedef struct vector_char_difference_data
{
    char delta_yaw;   /* 0x00 */
    char delta_pitch; /* 0x01 */
} vector_char_difference_data;
