#pragma once
// DB-verified via types_members _7AD7041248B2319AC41E71AD83B3A2E8 (anonymous PDB type)
/* terminal_globals @0x84429B34 — console/terminal output state (terminal.c). Owns the ring of output lines and
 * the active gets() input session. Layout taken verbatim from the database struct applied at the symbol. */

#include "console_globals.h"
#include "data_array.h"
#include "terminal_gets_state.h"

typedef struct terminal_globals_t
{
    unsigned char initialized;                 /* 0x00 */
    unsigned char _pad01[3];                   /* 0x01 */
    /* 0x01..0x03 padding */
    data_array *output_lines;                  /* 0x04: 292-byte line records */
    int newest_output_line_index;              /* 0x08 */
    int oldest_output_line_index;              /* 0x0C */
    terminal_gets_state *input_state;          /* 0x10: active gets() session, null when none */
    unsigned char insertion_point_visible;     /* 0x14 */
    unsigned char _pad15[3];                   /* 0x15 */
    /* 0x15..0x17 padding */
    int last_insertion_point_toggle;           /* 0x18 */
} terminal_globals_t;

extern terminal_globals_t terminal_globals;
