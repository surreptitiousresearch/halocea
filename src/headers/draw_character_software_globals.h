#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md.
 * Searched DB types for 'draw_character_software_globals'/'..._t': no hit. The two reversed consumers
 * (bitmap_draw_string writes .bitmap; bitmap_draw_character reads .bitmap and .encoding_shift) touch
 * only offsets 0x00 and 0x04, so the remaining members stay UNRESOLVED rather than invented. */
/* draw_character_software_globals @ 0x84469498 — scratch state for the software (CPU) bitmap glyph
 * rasterizer. bitmap_draw_string stashes the destination bitmap here so the per-character callback
 * (bitmap_draw_character) can reach it without threading it through draw_string. Only the .bitmap
 * member (offset 0x00) is exercised by the paths reversed so far; the remaining layout is unresolved. */

#include <stdint.h>
#include "bitmap_data.h"

typedef struct draw_character_software_globals_t
{
    bitmap_data *bitmap;    /* 0x00 */
    int16_t encoding_shift; /* 0x04 — left-shift applied to the destination x column (log2 bytes/pixel) */
    /* UNRESOLVED: remaining members (glyph cursor / colour state) — no DB type; not touched by any
     * reversed consumer (only .bitmap @0x00 and .encoding_shift @0x04 are used) */
} draw_character_software_globals_t;

#ifdef __cplusplus
extern "C" {
#endif

extern draw_character_software_globals_t draw_character_software_globals;

#ifdef __cplusplus
}
#endif
