#pragma once
/* output_line_datum — one debug-terminal output line record (292 bytes per DB types_members),
 * stored in terminal_globals.output_lines. Lines are chained newest-first via older/newer_line_index. */

#include <stdint.h>
#include "real_argb_color.h"

typedef struct output_line_datum
{
    int16_t         identifier;         /* 0x000 */
    int16_t         pad_02;             /* 0x002 */
    int             newer_line_index;   /* 0x004 */
    int             older_line_index;   /* 0x008 */
    uint8_t tabstop;            /* 0x00C */
    char            buffer[256];        /* 0x00D */
    char            pad_10D[3];         /* 0x10D — align color to 0x110 */
    real_argb_color color;              /* 0x110 (272) */
    int             timer;              /* 0x120 (288) */
} output_line_datum;                    /* 292 bytes */
