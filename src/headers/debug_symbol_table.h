#pragma once
/* debug_symbol_table — a loaded symbol table (stack-walk name resolution). Layout from the database
 * (12 bytes). */

#include "debug_symbol.h"

typedef struct debug_symbol_table
{
    int           number_of_symbols; /* 0x00 */
    char         *string_storage;    /* 0x04 */
    debug_symbol *symbols;           /* 0x08 */
} debug_symbol_table;                /* 12 bytes */
