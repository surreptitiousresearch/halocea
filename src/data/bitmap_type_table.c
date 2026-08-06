/* bitmap_type_table @ 0x8212A05C (.rdata, 8 bytes)
 * DB applied_types: const __int16 bitmap_type_table[4];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000: 0000 0002 0002 0002
 * .rdata, 8 bytes = 4 x uint16_t.
 * Element width: `slwi r9, r11, 1` + `lhzx r8, r9, r10` at 0x8382EBCC/0x8382EBD0 inside
 * rasterizer_dx9_transparent_chicago_draw (its only consumer) — 2-byte stride; the next .rdata
 * name, bitmap_address_table, starts at 0x8212A064, so exactly 4 entries.
 * Index: shader_transparent_chicago.type, the "first map type" enum (DB anonymous enum
 * $1587AB03BBD65F212ACF40B64423BA8C, 4 members). Values are bitmap_type constants: a 2d first map
 * binds a 2D bitmap, all three cube-map first-map types bind a cube map.
 * Raw bytes 0x8212A05C..0x8212A063: 00 00 00 02 00 02 00 02.
 * NOTE this is a distinct object from its ICF-lookalike twin bitmap_type_table_0 @0x8212A074;
 * both were dumped separately and only then found to hold the same four values.
 * /
 */
#include <stdint.h>
#include "bitmap_type.h"

const uint16_t bitmap_type_table[4] =
{
    bitmap_type_2d,        /* _shader_transparent_chicago_type_2d_map                          */
    bitmap_type_cube_map,  /* _shader_transparent_chicago_type_first_map_is_reflection_cube_map */
    bitmap_type_cube_map,  /* ..._first_map_is_object_centered_cube_map                        */
    bitmap_type_cube_map,  /* ..._first_map_is_viewer_centered_cube_map                        */
};
