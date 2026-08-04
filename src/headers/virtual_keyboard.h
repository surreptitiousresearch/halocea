#pragma once
/* virtual_keyboard — parsed virtual-keyboard tag definition: the fonts/bitmaps and the per-key
 * layout used to render and drive the on-screen keyboard. Layout from the database (60 bytes). */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct virtual_keyboard
{
    tag_reference font_tag;                           /* 0x00 */
    tag_reference background_bitmap_tag;              /* 0x10 */
    tag_reference special_key_labels_string_list_tag; /* 0x20 */
    tag_block     virtual_keys;                       /* 0x30 */
} virtual_keyboard;                                   /* 60 bytes */
