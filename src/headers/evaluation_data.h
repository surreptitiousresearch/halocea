#pragma once
/* evaluation_data — a 12-byte per-evaluator scratch block inside firing_position_evaluation_context.
 * DB models it as a struct wrapping one anonymous-union member ___u0 (_6F16B951CF56FD3C25914E5F8AF7F3B2)
 * of the three variant views (panic / pursue / cover). $-name kept verbatim ($ in identifiers is an
 * MSVC extension), matching the mtlPARAM.h convention. */

#include "panic_evaluation_data.h"
#include "pursue_evaluation_data.h"
#include "cover_evaluation_data.h"

/* DB union _6F16B951CF56FD3C25914E5F8AF7F3B2 — the three evaluator views overlaid (12 bytes). */
typedef union _6F16B951CF56FD3C25914E5F8AF7F3B2
{
    panic_evaluation_data  panic;    /* 0x0 (1 byte)   */
    pursue_evaluation_data pursue;   /* 0x0 (12 bytes) — sizes the union */
    cover_evaluation_data  cover;    /* 0x0 (1 byte)   */
} _6F16B951CF56FD3C25914E5F8AF7F3B2;

typedef struct evaluation_data
{
    union _6F16B951CF56FD3C25914E5F8AF7F3B2 ___u0; /* 0x0 */
} evaluation_data;               /* 12 bytes */
