#pragma once
/* hs_global_internal — one element of scenario.hs_globals (92 bytes; DB-verified). */

typedef struct hs_global_internal
{
    char             name[32];                        /* 0x00 */
    __int16          type;                            /* 0x20 */
    unsigned __int16 pad;                             /* 0x22 */
    int              pad2;                            /* 0x24 */
    int              initialization_expression_index; /* 0x28 */
    int              unused[12];                      /* 0x2C */
} hs_global_internal;                                 /* 92 bytes */
