#pragma once
/* hs_syntax_node — one HaloScript expression/syntax datum (20 bytes). Layout verbatim from the database type.
 * Stored in the hs_syntax_data data_array; index with HS_SYNTAX_NODE() below. */

/* DB anonymous union _CB762700E89E04DB3A4AAACD177AAC89 (2 bytes) — hs_syntax_node member ___u1
 * ($ in identifiers is an MSVC extension). */
typedef union _CB762700E89E04DB3A4AAACD177AAC89
{
    __int16 script_index;
    __int16 function_index;
    __int16 constant_type;
} _CB762700E89E04DB3A4AAACD177AAC89;

typedef struct hs_syntax_node
{
    __int16 identifier;                 /* 0x00 */
    union _CB762700E89E04DB3A4AAACD177AAC89 ___u1; /* 0x02 */
    __int16          type;              /* 0x04 */
    unsigned __int16 flags;             /* 0x06 */
    int              next_node_index;   /* 0x08 */
    int              source_offset;     /* 0x0C — also reused to hold a string-constant offset after hs_compile_finish */
    unsigned int     data;              /* 0x10 — long value, or first-argument expression index for a call */
} hs_syntax_node;

/* Datum access: the decompiler's "(unsigned __int16)idx + ((4*idx)&0x3FFFC)" dword offset reduces exactly to
 * 5*(unsigned __int16)idx dwords == one 20-byte node at element (unsigned __int16)idx. */
#define HS_SYNTAX_NODE(idx) (((hs_syntax_node *)hs_syntax_data->data)[(unsigned __int16)(idx)])
