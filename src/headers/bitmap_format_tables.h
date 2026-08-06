#pragma once

#include <stdint.h>
#include "d3d_render_boundary.h"   /* _D3DFORMAT, D3DFMT_* */

/* Bits-per-pixel per bitmap format (indexed by bitmap_data.format).
 * int8_t: every binary access is a stride-1 lbzx (15 sites, e.g. 0x836F436C) with extsb
 * normalization (0x836F4378) — a 16/32-bit element type would index with the wrong stride. */
#ifdef __cplusplus
extern "C" {
#endif

extern const int8_t bitmap_format_bits_per_pixel_table[];

/* Bitmap-format -> D3D hardware format mapping. */
extern _D3DFORMAT rasterizer_bitmap_format_table[];

/* String tables for bitmap metadata. */
extern const char *bitmap_type_string_table[];
extern const char *bitmap_format_string_table[];

#ifdef __cplusplus
}
#endif
