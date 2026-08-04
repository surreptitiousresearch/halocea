#pragma once
/* debug_symbol — one entry in a debug_symbol_table's symbol array. Layout from the database (16 bytes). */

typedef struct debug_symbol
{
    unsigned int address;                      /* 0x00 */
    unsigned int rva_base;                     /* 0x04 */
    unsigned int name_string_offset;           /* 0x08 */
    unsigned int library_object_string_offset; /* 0x0C */
} debug_symbol;                                /* 16 bytes */
