#pragma once
/* font_character_table_entry — one slot of a font character page: the index of the glyph in the
 * font tag's characters block, or -1 when the code point has no glyph (2 bytes; DB-verified,
 * matches headers_ref sizeof=0x2). */

#include <stdint.h>

typedef struct font_character_table_entry
{
    int16_t character_index;    /* 0x00 */
} font_character_table_entry;   /* 2 bytes */
