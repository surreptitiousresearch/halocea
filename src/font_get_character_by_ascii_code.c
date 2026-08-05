/* font_get_character_by_ascii_code @0x83812324 — resolve a Unicode code point to its cached glyph in a
 * font tag. The font's character_tables block is indexed by the upper bytes of the code point to find a
 * 256-entry page; a populated page (count==256) maps the low byte to a global character index, which in
 * turn selects a 20-byte font_character from the characters block. Returns nullptr when no glyph exists. */

#include <stdint.h>
#include "headers/font_header.h"
#include "headers/font_character.h"

font_character *font_get_character_by_ascii_code(font_header *header, uint16_t character)
{
    char *tables = (char *)header->character_tables.address;
    font_character *result = nullptr;

    /* select the page descriptor (count at +0, int16-index-table pointer at +4) by the high bytes */
    /* BE byte-2 of the code point = bits 15..8 (extrwi r4,8,16) = (character >> 8) & 0xFF */
    int *page = (int *)&tables[4 * ((character >> 8) & 0xFF) + 4 * ((character >> 7) & 0x1FE)];
    if (*page > 0)
    {
        int16_t *index_table = nullptr;
        if (*page == 256)
            index_table = (int16_t *)(((2 * character) & 0x1FE) + page[1]);

        int character_index = *index_table;
        if (character_index != -1)
            /* recovered: (char *)characters.address + 20*idx -> characters[idx] (stride 20 == sizeof(font_character)) */
            return &((font_character *)header->characters.address)[character_index];
    }
    return result;
}
