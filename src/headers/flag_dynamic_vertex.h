#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* flag_dynamic_vertex — one dynamic-geometry vertex for a rendered flag's cloth mesh (68 bytes). Layout
 * derived from disasm (flag_render_proper.c): position and tangent are DB-shaped floats; bytes 0x18-0x2F
 * and the word pair at 0x38 are never written by that function and are left unresolved (likely a second
 * attribute stream the flag shader doesn't use). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct flag_dynamic_vertex
{
    real_point3d  position;         /* 0x00 */
    real_vector3d tangent;          /* 0x0C — cross-product tangent/normal, normalized when non-degenerate */
    unsigned char unused18[24];     /* 0x18 */
    /* DEVIATION: an earlier pass had these two swapped. Disasm at 0x8380E490-0x8380E51C proves
     * column_fraction uses 1/(width-1) (the outer/column loop, computed once per column) and
     * row_fraction uses 1/(height-1) (the inner/row loop) — confirmed by tracing which divide result
     * (f6 vs f7) each one multiplies. */
    float         column_fraction;  /* 0x30 — column / (width - 1) */
    float         row_fraction;     /* 0x34 — row / (height - 1) */
    unsigned char unused38[4];      /* 0x38 */
    float         unused3C;         /* 0x3C — always 0.5 */
    float         unused40;         /* 0x40 — always 0.5 */
} flag_dynamic_vertex; /* 68 bytes */
