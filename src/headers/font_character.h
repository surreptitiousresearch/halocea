#pragma once
/* font_character — a glyph entry in a font tag (20 bytes). Layout from the database. */

typedef struct font_character
{
    unsigned __int16 character;                /* 0x00 */
    __int16          character_width;          /* 0x02 */
    __int16          bitmap_width;             /* 0x04 */
    __int16          bitmap_height;            /* 0x06 */
    __int16          bitmap_origin_x;          /* 0x08 */
    __int16          bitmap_origin_y;          /* 0x0A */
    __int16          hardware_character_index; /* 0x0C — slot in the hardware character cache, or -1 */
    unsigned __int16 pad;                      /* 0x0E */
    int              pixels_offset;            /* 0x10 */
} font_character;
