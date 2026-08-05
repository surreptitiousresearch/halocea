#pragma once
#include <stdint.h>
/* hs_global_internal — one element of scenario.hs_globals (92 bytes; DB-verified). */

typedef struct hs_global_internal
{
    char             name[32];                        /* 0x00 */
    int16_t          type;                            /* 0x20 */
    uint16_t pad;                             /* 0x22 */
    int              pad2;                            /* 0x24 */
    int              initialization_expression_index; /* 0x28 */
    int              unused[12];                      /* 0x2C */
} hs_global_internal;                                 /* 92 bytes */
