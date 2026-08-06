#pragma once
/* font_character_tables_entry — one page of a font tag's character_tables block (12 bytes;
 * DB-verified, matches headers_ref sizeof=0xC). The page is selected by the code point's high
 * byte; a populated page carries a 256-entry font_character_table_entry block. */

#include "tag_block.h"

typedef struct font_character_tables_entry
{
    tag_block table;    /* 0x00 — font_character_table_entry[] */
} font_character_tables_entry;   /* 12 bytes */
