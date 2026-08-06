/* font_get_character_by_ascii_code @0x83812324 — resolve a Unicode code point to its cached glyph in a
 * font tag. The font's character_tables block is indexed by the code point's high byte to find a page;
 * a populated page (count==256) maps the low byte to a global character index, which in turn selects a
 * 20-byte font_character from the characters block. Returns nullptr when no glyph exists.
 *
 * DEVIATION: the decompiler split the page-table subscript into `4 * ((character >> 8) & 0xFF) +
 * 4 * ((character >> 7) & 0x1FE)` over a `char *`. Those two terms are the same high byte scaled by 4
 * and by 8 (disasm 0x83812328 `rlwinm r11, r4, 25,23,30` is exactly ((character >> 8) & 0xFF) * 2), so
 * the fold is one 12-byte font_character_tables_entry stride, not two additions.
 * The high-byte extract itself is correct as decompiled — 0x83812320 `extrwi r10, r4, 8,16` takes bits
 * 15..8 of the zero-extended code point. */

#include <stdint.h>
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/font_character_tables_entry.h"
#include "headers/font_character_table_entry.h"

font_character *font_get_character_by_ascii_code(font_header *header, uint16_t character)
{
    font_character_tables_entry *character_tables =
        (font_character_tables_entry *)header->character_tables.address;
    font_character *result = nullptr;

    font_character_tables_entry *page = &character_tables[(character >> 8) & 0xFF];
    if (page->table.count > 0)
    {
        font_character_table_entry *index_table = nullptr;
        if (page->table.count == 256)
            index_table = &((font_character_table_entry *)page->table.address)[character & 0xFF];

        /* Faithful to the binary: a page with a count that is neither 0 nor 256 falls through to
         * `li r11, 0` / `lhz r11, 0(r11)` at 0x83812368 and loads through a null pointer. The tag
         * format only ever emits 0 or 256, so the path is dead. */
        int character_index = index_table->character_index;
        if (character_index != -1)
            /* recovered: (char *)characters.address + 20*idx -> characters[idx] (stride 20 == sizeof(font_character)) */
            return &((font_character *)header->characters.address)[character_index];
    }
    return result;
}
