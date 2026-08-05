/* bitmap_address_table_0 @0x8212A07C — .rdata, 4 x int. The shader_transparent_chicago_extended
 * copy of bitmap_address_table (ICF twin by content, separate object: reconstructed from its own
 * bytes at its own address).
 * Element width: `slwi r9, r10, 2` + `lwzx r7, r9, r24` at 0x8382F45C/0x8382F460 inside
 * rasterizer_dx9_transparent_chicago_extended_draw (its only consumer) — 4-byte stride.
 * Extent: the next .rdata name is anisoFilterMap_65 @0x8212A090, i.e. 20 bytes of span, but the
 * object is 4 entries: raw bytes 0x8212A07C..0x8212A08B are 00000000 00000002 00000002 00000002
 * and 0x8212A08C..0x8212A08F are zero with no xref — alignment padding that brings the
 * 16-byte-aligned anisoFilterMap_65 onto its boundary. Not emitted. The non-extended twin
 * bitmap_address_table is 4 entries with no such pad, and the consumer's index domain (the
 * 4-member first-map-type enum) admits no 5th entry.
 * Values are Xbox 360 _D3DTEXTUREADDRESS modes fed to D3DDevice_SetSamplerState_Address{U,V,W}.
 */

#include "_D3DTEXTUREADDRESS.h"

const int bitmap_address_table_0[4] =
{
    D3DTADDRESS_WRAP,   /* _shader_transparent_chicago_type_2d_map                          */
    D3DTADDRESS_CLAMP,  /* _shader_transparent_chicago_type_first_map_is_reflection_cube_map */
    D3DTADDRESS_CLAMP,  /* ..._first_map_is_object_centered_cube_map                        */
    D3DTADDRESS_CLAMP,  /* ..._first_map_is_viewer_centered_cube_map                        */
};
