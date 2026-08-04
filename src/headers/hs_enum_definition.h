#pragma once
/* hs_enum_definition — a HaloScript enum type: a count and a parallel array of identifier strings. Layout from
 * the database `hs_enum_definition` type. */

typedef struct hs_enum_definition
{
    __int16       count;        /* 0x0 */
    unsigned char _pad0[2]; /* db-verified padding */
    const char  **identifiers;  /* 0x4 — `count` identifier strings */
} hs_enum_definition;

/* Indexed by (script type - 32); the enum types are the contiguous block starting at type 32. */
extern const hs_enum_definition hs_enum_table[];
