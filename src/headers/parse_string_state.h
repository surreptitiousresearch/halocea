#pragma once
/* parse_string_state — cursor/state for the markup string parser used by the text layout/draw code. Each
 * parse_string() call advances through the string, decoding the next character or control token (newline,
 * tab, paragraph, style/color change, ...) into `character`/`result`. Layout from the database (28 bytes). */

#include <stdint.h>
#include "font_header.h"

typedef struct parse_string_state
{
    int              base_font_index; /* 0x00 */
    font_header     *font_header;     /* 0x04 */
    uint8_t *string;          /* 0x08 */
    int16_t          string_index;    /* 0x0C */
    int16_t          style;           /* 0x0E */
    int16_t          justification;   /* 0x10 */
    uint16_t character;       /* 0x12 */
    int16_t          result;          /* 0x14 */
    uint16_t _pad;            /* 0x16 */
    unsigned int     color;           /* 0x18 */
} parse_string_state;                 /* 0x1C */
