#pragma once
#include <stdint.h>
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* hs_global_definition — HaloScript engine (external) global descriptor. Only the name is referenced by
 * reconstructed callers (type at +0, name at +4). */

typedef struct hs_global_definition
{
    int16_t  type; /* 0x0 */
    char    *name; /* 0x4 */
} hs_global_definition;
