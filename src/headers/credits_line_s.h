#pragma once
#include <stddef.h> /* wchar_t is not a builtin type in C */

/* credits_line_s — element of the scrolling-credits dynamic_array (20 bytes, DB-verified). */

typedef struct credits_line_s
{
    const wchar_t *string;        /* 0x00 */
    int            string_index;  /* 0x04 */
    int            formatting;    /* 0x08 */
    int            y0;            /* 0x0C */
    int            y1;            /* 0x10 */
} credits_line_s;                 /* 20 bytes */
