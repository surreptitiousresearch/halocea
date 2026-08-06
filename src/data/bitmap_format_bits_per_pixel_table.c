/* bitmap_format_bits_per_pixel_table @ 0x82113EC0 (.rdata, 19 bytes)
 * DB applied_types: const char bitmap_format_bits_per_pixel_table[19];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000: 08 08 08 10 00 00 10 00 10 10 20 20 00 00 04 08
 *   +0x0010: 08 08 FF
 * .rdata bits-per-pixel per bitmap_format.
 * Element width: `lbzx r8, r10, r9` + `extsb r3, r8` at 0x836F427C/0x836F4280
 * (bitmap_format_get_bits_per_pixel), i.e. a signed byte with stride 1 — the 15 access sites in
 * the binary all load it that way.
 * Extent: raw bytes 0x82113EC0..0x82113ED3 are
 *   08 08 08 10 00 00 10 00 10 10 20 20 00 00 04 08 08 08 FF 00
 * and the next .rdata name (a "D:\..." path string) starts at 0x82113ED4. Bytes [0..17] are the
 * 18 format entries, [18] = 0xFF is the -1 terminator (MSVC pads .rdata with 0x00, never 0xFF —
 * and srcblend_table/destblend_table/blendop_table carry the same -1 terminator convention), and
 * [19] = 0x00 is the single alignment pad byte before the string, so it is not emitted.
 * DXT block formats report their amortized per-texel cost (dxt1 = 4, dxt3/dxt5 = 8).
 * /
 */
#include <stdint.h>
#include "bitmap_format.h"

const int8_t bitmap_format_bits_per_pixel_table[NUMBER_OF_BITMAP_FORMATS + 1] =
{
     8,   /* _bitmap_format_a8       */
     8,   /* _bitmap_format_y8       */
     8,   /* _bitmap_format_ay8      */
    16,   /* _bitmap_format_a8y8     */
     0,   /* _bitmap_format_unused1  */
     0,   /* _bitmap_format_unused2  */
    16,   /* _bitmap_format_r5g6b5   */
     0,   /* _bitmap_format_unused3  */
    16,   /* _bitmap_format_a1r5g5b5 */
    16,   /* _bitmap_format_a4r4g4b4 */
    32,   /* _bitmap_format_x8r8g8b8 */
    32,   /* _bitmap_format_a8r8g8b8 */
     0,   /* _bitmap_format_unused4  */
     0,   /* _bitmap_format_unused5  */
     4,   /* _bitmap_format_dxt1     */
     8,   /* _bitmap_format_dxt3     */
     8,   /* _bitmap_format_dxt5     */
     8,   /* _bitmap_format_p8_bump  */
    -1,   /* 0xFF terminator */
};
