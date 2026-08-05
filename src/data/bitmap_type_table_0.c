/* bitmap_type_table_0 @0x8212A074 — .rdata, 8 bytes = 4 x uint16_t. The
 * shader_transparent_chicago_extended copy of bitmap_type_table (ICF twin by content, separate
 * object: reconstructed from its own bytes at its own address).
 * Element width: `slwi r9, r11, 1` + `lhzx r8, r9, r10` at 0x8382F400/0x8382F404 inside
 * rasterizer_dx9_transparent_chicago_extended_draw (its only consumer) — 2-byte stride; the next
 * .rdata name, bitmap_address_table_0, starts at 0x8212A07C, so exactly 4 entries.
 * Index: shader_transparent_chicago_extended.type, the "first map type" enum (4 members).
 * Raw bytes 0x8212A074..0x8212A07B: 00 00 00 02 00 02 00 02.
 */

#include <stdint.h>
#include "bitmap_type.h"

const uint16_t bitmap_type_table_0[4] =
{
    bitmap_type_2d,        /* _shader_transparent_chicago_type_2d_map                          */
    bitmap_type_cube_map,  /* _shader_transparent_chicago_type_first_map_is_reflection_cube_map */
    bitmap_type_cube_map,  /* ..._first_map_is_object_centered_cube_map                        */
    bitmap_type_cube_map,  /* ..._first_map_is_viewer_centered_cube_map                        */
};
