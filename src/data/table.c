/* table @ 0x82113CD8 (.rdata, 72 bytes)
 * DB applied_types: const _D3DFORMAT table[18];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = 0xFFFFFFFF
 *   +0x0004 = 0xFFFFFFFF
 *   +0x0008 = 0xFFFFFFFF
 *   +0x000C = 0xFFFFFFFF
 *   +0x0010 = 0xFFFFFFFF
 *   +0x0014 = 0xFFFFFFFF
 *   +0x0018 = 0x28280144
 *   +0x001C = 0xFFFFFFFF
 *   +0x0020 = 0x18280143
 *   +0x0024 = 0x1828014F
 *   +0x0028 = 0x28280186
 *   +0x002C = 0x18280186
 *   +0x0030 = 0xFFFFFFFF
 *   +0x0034 = 0xFFFFFFFF
 *   +0x0038 = 0x1A200152
 *   +0x003C = 0x1A200153
 *   +0x0040 = 0x1A200154
 *   +0x0044 = 0xFFFFFFFF
 * .rdata bitmap_format -> Xbox 360 _D3DFORMAT lookup, 72 bytes = 18 x 4.
 * The symbol really is named `table` in the binary: it is the file-scope table of
 * bitmap_format_to_d3d_format (0x836F4138), whose whole body is
 * `extsh r11,r3 / slwi r9,r11,2 / lwzx r3,r9,r8` — a 4-byte stride indexed by bitmap_data.format,
 * which fixes the element count at 18 == NUMBER_OF_BITMAP_FORMATS (next .rdata name @0x82113D20).
 * Byte-for-byte identical to rasterizer_bitmap_format_table @0x82123B10, but a distinct object
 * (both were dumped from the binary; neither was assumed from the other).
 * NOTE on the -1 slots: this file deliberately spells them `(_D3DFORMAT)-1` rather than
 * D3DFMT_UNKNOWN. _D3DFORMAT.h's real enumerator D3DFMT_UNKNOWN is -1, but d3d_render_boundary.h
 * defines a same-named legacy macro worth 0; the raw-sentinel spelling reads the same whichever
 * header a future reader has pulled in, and matches rasterizer_bitmap_new.c's existing compare.
 *   [ 0..5]  0xFFFFFFFF -> -1   (a8, y8, ay8, a8y8, unused1, unused2 have no 360 hw format)
 *   [ 6]     0x28280144 -> D3DFMT_R5G6B5      [ 7]     0xFFFFFFFF -> -1 (unused3)
 *   [ 8]     0x18280143 -> D3DFMT_A1R5G5B5    [ 9]     0x1828014F -> D3DFMT_A4R4G4B4
 *   [10]     0x28280186 -> D3DFMT_X8R8G8B8    [11]     0x18280186 -> D3DFMT_A8R8G8B8
 *   [12,13]  0xFFFFFFFF -> -1 (unused4, unused5)
 *   [14]     0x1A200152 -> D3DFMT_DXT1        [15]     0x1A200153 -> D3DFMT_DXT3
 *   [16]     0x1A200154 -> D3DFMT_DXT5        [17]     0xFFFFFFFF -> -1 (p8_bump)
 * /
 */
#include "_D3DFORMAT.h"
#include "bitmap_format.h"

_D3DFORMAT table[NUMBER_OF_BITMAP_FORMATS] =
{
    (_D3DFORMAT)-1,     /* _bitmap_format_a8       */
    (_D3DFORMAT)-1,     /* _bitmap_format_y8       */
    (_D3DFORMAT)-1,     /* _bitmap_format_ay8      */
    (_D3DFORMAT)-1,     /* _bitmap_format_a8y8     */
    (_D3DFORMAT)-1,     /* _bitmap_format_unused1  */
    (_D3DFORMAT)-1,     /* _bitmap_format_unused2  */
    D3DFMT_R5G6B5,      /* _bitmap_format_r5g6b5   */
    (_D3DFORMAT)-1,     /* _bitmap_format_unused3  */
    D3DFMT_A1R5G5B5,    /* _bitmap_format_a1r5g5b5 */
    D3DFMT_A4R4G4B4,    /* _bitmap_format_a4r4g4b4 */
    D3DFMT_X8R8G8B8,    /* _bitmap_format_x8r8g8b8 */
    D3DFMT_A8R8G8B8,    /* _bitmap_format_a8r8g8b8 */
    (_D3DFORMAT)-1,     /* _bitmap_format_unused4  */
    (_D3DFORMAT)-1,     /* _bitmap_format_unused5  */
    D3DFMT_DXT1,        /* _bitmap_format_dxt1     */
    D3DFMT_DXT3,        /* _bitmap_format_dxt3     */
    D3DFMT_DXT5,        /* _bitmap_format_dxt5     */
    (_D3DFORMAT)-1,     /* _bitmap_format_p8_bump  */
};
