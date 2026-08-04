#pragma once
/* parse_string_state — cursor/state for the markup string parser used by the text layout/draw code. Each
 * parse_string() call advances through the string, decoding the next character or control token (newline,
 * tab, paragraph, style/color change, ...) into `character`/`result`. Layout from the database (28 bytes). */

#include "font_header.h"

typedef struct parse_string_state
{
    int              base_font_index; /* 0x00 */
    font_header     *font_header;     /* 0x04 */
    unsigned __int8 *string;          /* 0x08 */
    __int16          string_index;    /* 0x0C */
    __int16          style;           /* 0x0E */
    __int16          justification;   /* 0x10 */
    unsigned __int16 character;       /* 0x12 */
    __int16          result;          /* 0x14 */
    unsigned __int16 _pad;            /* 0x16 */
    unsigned int     color;           /* 0x18 */
} parse_string_state;                 /* 0x1C */
