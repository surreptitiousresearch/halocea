/* bitmap_address_table @0x8212A064 — .rdata, 16 bytes = 4 x int.
 * Element width: `slwi r9, r10, 2` + `lwzx r7, r9, r23` at 0x8382EC28/0x8382EC2C inside
 * rasterizer_dx9_transparent_chicago_draw (its only consumer) — 4-byte stride; the next .rdata
 * name, bitmap_type_table_0, starts at 0x8212A074, so exactly 4 entries and no padding.
 * Index: shader_transparent_chicago.type, the "first map type" enum (DB anonymous enum
 * $1587AB03BBD65F212ACF40B64423BA8C, 4 members). Values are Xbox 360 _D3DTEXTUREADDRESS modes fed
 * to D3DDevice_SetSamplerState_Address{U,V,W}: a 2d first map wraps, the cube-map first-map types
 * clamp. The same consumer passes a literal 2 on its u/v-clamped arms, which pins CLAMP == 2.
 * Raw bytes 0x8212A064..0x8212A073: 00000000 00000002 00000002 00000002.
 * NOTE this is a distinct object from its ICF-lookalike twin bitmap_address_table_0 @0x8212A07C
 * (which is followed by 4 bytes of alignment padding); both were dumped separately.
 */

#include "_D3DTEXTUREADDRESS.h"

const int bitmap_address_table[4] =
{
    D3DTADDRESS_WRAP,   /* _shader_transparent_chicago_type_2d_map                          */
    D3DTADDRESS_CLAMP,  /* _shader_transparent_chicago_type_first_map_is_reflection_cube_map */
    D3DTADDRESS_CLAMP,  /* ..._first_map_is_object_centered_cube_map                        */
    D3DTADDRESS_CLAMP,  /* ..._first_map_is_viewer_centered_cube_map                        */
};
