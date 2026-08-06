#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* text_bounds_globals — scratch state used by the text-bounds-measuring draw callback: the accumulated
 * bounding rectangle and the font header in effect for the last drawn character. Layout from the
 * database (12 bytes). */

#include "rectangle2d.h"
#include "font_header.h"

typedef struct text_bounds_globals_t
{
    rectangle2d  bounds;           /* 0x00 — accumulated min/max extents */
    font_header *last_font_header; /* 0x08 */
} text_bounds_globals_t;

#ifdef __cplusplus
extern "C" {
#endif

extern text_bounds_globals_t text_bounds_globals;

#ifdef __cplusplus
}
#endif
