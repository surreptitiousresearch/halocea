#pragma once
#include <stdint.h>
/* font_character — a glyph entry in a font tag (20 bytes). Layout from the database. */

typedef struct font_character
{
    uint16_t character;                /* 0x00 */
    int16_t          character_width;          /* 0x02 */
    int16_t          bitmap_width;             /* 0x04 */
    int16_t          bitmap_height;            /* 0x06 */
    int16_t          bitmap_origin_x;          /* 0x08 */
    int16_t          bitmap_origin_y;          /* 0x0A */
    int16_t          hardware_character_index; /* 0x0C — slot in the hardware character cache, or -1 */
    uint16_t pad;                      /* 0x0E */
    int              pixels_offset;            /* 0x10 */
} font_character;
