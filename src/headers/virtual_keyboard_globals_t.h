#pragma once
#include <stdint.h>
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* virtual_keyboard_globals — on-screen keyboard runtime state. Layout from the database
 * (type _virtual_keyboard_globals). */

#include "virtual_keyboard.h"

// DB-verified via types_members _virtual_keyboard_globals (Blam spelling: virtual_keyboard_globals_t)
typedef struct _virtual_keyboard_globals
{
    uint8_t  active;                    /* 0x00 */
    uint8_t  shift_active;              /* 0x01 */
    uint8_t  caps_active;               /* 0x02 */
    uint8_t  symbols_active;            /* 0x03 */
    struct virtual_keyboard *keyboard;          /* 0x04 — `struct` prefix avoids the verifier's `virtual` keyword skip */
    int16_t          row;                       /* 0x08 */
    int16_t          column;                    /* 0x0A */
    uint16_t buffer_size;               /* 0x0C */
    int16_t          last_event;                /* 0x0E */
    int16_t          last_key;                  /* 0x10 */
    uint16_t number_of_event_repeats;   /* 0x12 */
    int16_t          caption_index;             /* 0x14 */
    uint8_t  buffer_saved;              /* 0x16 */
    uint8_t  first_key_replaces_buffer; /* 0x17 */
    uint16_t *text_buffer;              /* 0x18 */
    uint16_t *insertion_point;          /* 0x1C */
    unsigned int     time_of_last_event;        /* 0x20 */
    int              caret_bitmap_tag;          /* 0x24 */
    wchar_t          original_buffer[32];       /* 0x28 */
    int              validate_mode;             /* 0x68 */
    int              caption_font_tag;          /* 0x6C */
    int              text_font_tag;             /* 0x70 */
} virtual_keyboard_globals_t;

extern virtual_keyboard_globals_t virtual_keyboard_globals;
