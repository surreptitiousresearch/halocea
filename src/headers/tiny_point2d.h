#pragma once
/* tiny_point2d — packed signed byte 2D point (motion-sensor blip coordinate). */

typedef struct tiny_point2d
{
    char x; /* 0x00 */
    char y; /* 0x01 */
} tiny_point2d; /* 2 bytes */
