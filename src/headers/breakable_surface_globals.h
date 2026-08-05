/* breakable_surface_globals — per-BSP breakable-surface state. Per-BSP destroyed-surface flag
 * bitsets (one 32-byte bitset per structure BSP, up to 16) plus per-surface vitality data. */
#pragma once
#include <stdint.h>
#include "breakable_surface_datum.h"

typedef struct breakable_surface_globals
{
    unsigned char enabled;                      /* 0x000 */
    unsigned char breakable_surface_flags[16][32]; /* 0x001 — destroyed-surface bitset, one per BSP */
    char          _pad201[3];                    /* 0x201 — align to 0x204 */
    breakable_surface_datum breakable_surfaces[16][256]; /* 0x204 — per-surface vitality (1024-byte stride/BSP) */
} breakable_surface_globals; /* 16900 bytes */

extern struct breakable_surface_globals *globals;
extern int16_t global_structure_bsp_index;
