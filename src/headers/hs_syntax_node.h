#pragma once
#include <stdint.h>
/* hs_syntax_node — one HaloScript expression/syntax datum (20 bytes). Layout verbatim from the database type.
 * Stored in the hs_syntax_data data_array; index with HS_SYNTAX_NODE() below. */

/* DB anonymous union _CB762700E89E04DB3A4AAACD177AAC89 (2 bytes) — hs_syntax_node member ___u1
 * ($ in identifiers is an MSVC extension). */
typedef union _CB762700E89E04DB3A4AAACD177AAC89
{
    int16_t script_index;
    int16_t function_index;
    int16_t constant_type;
} _CB762700E89E04DB3A4AAACD177AAC89;

typedef struct hs_syntax_node
{
    int16_t identifier;                 /* 0x00 */
    union _CB762700E89E04DB3A4AAACD177AAC89 ___u1; /* 0x02 */
    int16_t          type;              /* 0x04 */
    uint16_t flags;             /* 0x06 */
    int              next_node_index;   /* 0x08 */
    int              source_offset;     /* 0x0C — also reused to hold a string-constant offset after hs_compile_finish */
    unsigned int     data;              /* 0x10 — long value, or first-argument expression index for a call */
} hs_syntax_node;

/* Datum access: the decompiler's "(unsigned __int16)idx + ((4*idx)&0x3FFFC)" dword offset reduces exactly to
 * 5*(unsigned __int16)idx dwords == one 20-byte node at element (unsigned __int16)idx. */
#define HS_SYNTAX_NODE(idx) (((hs_syntax_node *)hs_syntax_data->data)[(uint16_t)(idx)])
