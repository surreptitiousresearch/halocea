#pragma once

#include "debug_symbol_table.h"

typedef struct _stack_walk_globals
{
    int                 fixup;                  /* 0x00 */
    unsigned char       disregard_symbol_names; /* 0x04 */
    unsigned char       _pad05[3];
    debug_symbol_table  symbol_table;           /* 0x08 (12 bytes) */
} _stack_walk_globals;                          /* 20 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern _stack_walk_globals stack_walk_globals;

#ifdef __cplusplus
}
#endif
