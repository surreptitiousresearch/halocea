/* bitmap_2d_get_pixel @0x836F4C98 — sample a single texel of a 2D bitmap from its CPU-side pixel data at a
 * given LOD, returning it expanded to a 32-bit color (or -1 if the bitmap has no pixels). Picks a mip level
 * from `lod` (0 when lod >= 1 or the bitmap is unmipmapped), derives the mip's width/height (padded up to a
 * multiple of 4 for compressed bitmaps), wraps the U/V texel coordinates (power-of-2 fast path via `&`, else
 * `%`), then reads the texel: S3TC-block-decode for compressed formats, a swizzled fetch when the swizzle flag
 * is set, or a straight linear fetch — the last two via bitmap_format_to_a8r8g8b8. The CPU-data sibling of
 * _rasterizer_bitmap_2d_get_pixel (which locks the hardware texture instead).
 *
 * Flagged "local variable allocation has failed"; every field offset, call argument and branch below was
 * re-derived from disassembly (0x836F4CAC-0x836F4FF0), not trusted from the decompiler's OVERLAPPED-register
 * display. In particular: the two `point` reads punned through HIDWORD(v5) are point->n[0]/n[1]; the duplicate
 * __int64-punned width/height float multiplies collapse to one raw-coordinate computation per axis; and the
 * swizzle path's `v27 | v26` is the OR of the two dwords bitmap_swizzle_vector2d writes into its result[2].
 *
 * FAITHFUL QUIRK: in the compressed path a format other than 14/15/16 returns the pixel accumulator
 * uninitialized (the decoder is simply not called), reproduced as-is (disasm 0x836F4F44-0x836F4F58). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"
#include "headers/bitmap_format.h"
#include "headers/real_point2d.h"
#include "headers/s3tc_block.h"

extern const int8_t bitmap_format_bits_per_pixel_table[];  /* lbzx+extsb stride-1 byte table (all 15 binary access sites) */

extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern unsigned int bitmap_format_to_a8r8g8b8(int16_t format, void *mipmap_address, int pixel_index);
extern void bitmap_swizzle_vector2d(int16_t dim_x, int16_t dim_y, int16_t x, int16_t y, int *result);
extern void DecodeBlockRGB__single_pixel(const S3TCBlockRGB *pblockSrc, S3TC_COLOR *colorDst, int16_t u, int16_t v);
extern void DecodeBlockAlpha4__single_pixel(const S3TCBlockAlpha4 *pblockSrc, S3TC_COLOR *colorDst, int16_t u, int16_t v);
extern void DecodeBlockAlpha3__single_pixel(const S3TCBlockAlpha3 *pblockSrc, S3TC_COLOR *colorDst, int16_t u, int16_t v);

unsigned int bitmap_2d_get_pixel(const bitmap_data *bitmap, const real_point2d *point, float lod)
{
    if (!bitmap->base_address)
        return -1;

    int16_t mip_level;
    if (lod >= 1.0f || bitmap->mipmap_count <= 0)
        mip_level = 0;
    else
        mip_level = (int16_t)(int)(((1.0f - lod) * (float)bitmap->mipmap_count));

    uint16_t flags = bitmap->flags;

    int16_t width = bitmap->width >> mip_level;
    if (width <= 1)
        width = 1;
    if (flags & (1u << _bitmap_compressed_bit))
        width = (int16_t)(((-width) & 3) + width);   /* compressed: pad to a multiple of 4 */

    int16_t height = bitmap->height >> mip_level;
    if (height <= 1)
        height = 1;
    if (flags & (1u << _bitmap_compressed_bit))
        height = (int16_t)(((-height) & 3) + height);

    int raw_u = (int)(((float)width * point->n[0]) - 0.5f);
    int16_t u = ((width - 1) & width) ? (int16_t)(((raw_u % width) + width) % width)
                                      : (int16_t)(raw_u & (width - 1));

    int raw_v = (int)(((float)height * point->n[1]) - 0.5f);
    int16_t v = ((height - 1) & height) ? (int16_t)(((raw_v % height) + height) % height)
                                        : (int16_t)(raw_v & (height - 1));

    char *base = bitmap_mipmap_address(bitmap, mip_level);

    if (flags & (1u << _bitmap_compressed_bit))
    {
        /* compressed (S3TC): locate the 4x4 block, then decode the single texel within it */
        int16_t block_u = u & 3;
        int16_t block_v = v & 3;
        int16_t block_size = (int16_t)((16 * bitmap_format_bits_per_pixel_table[bitmap->format]) >> 3);
        const void *block = &base[(v / 4 * width / 4 + u / 4) * block_size];

        unsigned int pixel;   /* FAITHFUL QUIRK: uninitialized for formats other than 14/15/16 */
        switch ((uint16_t)bitmap->format)
        {
            case _bitmap_format_dxt1:
                DecodeBlockRGB__single_pixel((const S3TCBlockRGB *)block, (S3TC_COLOR *)&pixel, block_u, block_v);
                break;
            case _bitmap_format_dxt3:
                DecodeBlockAlpha4__single_pixel((const S3TCBlockAlpha4 *)block, (S3TC_COLOR *)&pixel, block_u,
                        block_v);
                break;
            case _bitmap_format_dxt5:
                DecodeBlockAlpha3__single_pixel((const S3TCBlockAlpha3 *)block, (S3TC_COLOR *)&pixel, block_u,
                        block_v);
                break;
        }
        return pixel;
    }

    if (flags & (1u << _bitmap_swizzled_bit))
    {
        /* swizzled: fetch the swizzled byte offset, then expand */
        int swizzle_offset[2];
        bitmap_swizzle_vector2d(width, height, u, v, swizzle_offset);
        return bitmap_format_to_a8r8g8b8(bitmap->format, base, swizzle_offset[0] | swizzle_offset[1]);
    }

    /* linear */
    return bitmap_format_to_a8r8g8b8(bitmap->format, base, v * width + u);
}
