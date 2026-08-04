#pragma once
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* virtual_key — one key definition in a virtual_keyboard's key layout (80 bytes, DB layout): the raw key
 * code plus all 7 shift/caps/symbols character variants and their background bitmaps. */

#include "tag_reference.h"

typedef struct virtual_key
{
    unsigned __int16 virtual_key;                     /* 0x00 */
    wchar_t           lowercase_character;             /* 0x02 */
    wchar_t           shift_character;                 /* 0x04 */
    wchar_t           caps_character;                  /* 0x06 */
    wchar_t           symbols_character;                /* 0x08 */
    wchar_t           shift_caps_character;             /* 0x0A */
    wchar_t           shift_symbols_character;          /* 0x0C */
    wchar_t           caps_symbols_character;           /* 0x0E */
    tag_reference     unselected_background_bitmap_tag; /* 0x10 */
    tag_reference     selected_background_bitmap_tag;   /* 0x20 */
    tag_reference     active_background_bitmap_tag;     /* 0x30 */
    tag_reference     sticky_background_bitmap_tag;     /* 0x40 */
} virtual_key;                                          /* 80 bytes */
