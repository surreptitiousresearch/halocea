/* _rasterizer_bitmap_2d_get_pixel @0x837937B8 — sample a single texel of a 2D bitmap at a given LOD,
 * returning it expanded to a 32-bit color. Picks a mip level from `lod` (clamped to the texture's real
 * level count), wraps the U/V texel coordinates (power-of-2 fast path via `&`, else `%`), locks the
 * chosen mip level, then either S3TC-block-decodes a single pixel (compressed bitmaps) or reads/expands
 * a raw pixel format directly. Flagged by the decompiler with "local variable allocation has failed";
 * every call argument and field offset below was re-derived from disassembly, not trusted from the
 * decompiler's own display.
 *
 * DEVIATION: `D3DBaseTexture_GetLevelCount`'s bogus 0x82000000/`HIDWORD` noise and the width/height
 * power-of-2 test's `__int64`-punned duplicate float reads are decompiler artifacts (the same raw texel
 * coordinate is computed identically in both the masked and modulo branches); simplified to one shared
 * computation per axis, matching the established pattern for this class of artifact elsewhere in this
 * codebase.
 *
 * FAITHFUL QUIRK: if `D3DTexture_LockRect` fails (`pBits == NULL`), the function returns the pixel
 * accumulator local uninitialized — genuinely never assigned in this path, not a transcription gap.
 * Also: the raw-format 8-bits-per-pixel read path indexes by `4 * u` (matching the 32-bit path's
 * stride), not `1 * u`; confirmed via disasm, reproduced as-is.
 *
 * The raw-format pixel remap for the three 16-bit formats (`case 2`/`case 3`/default's `v32==0` arm)
 * reuses the exact bit-repack expressions already verified against disassembly for the equivalent
 * A1R5G5B5 / A4R4G4B4 / R5G6B5 cases in the sibling `bitmap_format_to_a8r8g8b8.c`. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"
#include "headers/bitmap_format.h"
#include "headers/real_point2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/hexrays_defs.h"
#include "headers/blam_data_globals.h"

typedef struct S3TCBlockRGB S3TCBlockRGB;
typedef struct S3TCBlockAlpha4 S3TCBlockAlpha4;
typedef struct S3TCBlockAlpha3 S3TCBlockAlpha3;
typedef struct S3TC_COLOR S3TC_COLOR;


extern void *hcex_tex_gethw(void *hardware_format, const char *name);
extern unsigned int D3DBaseTexture_GetLevelCount(D3DBaseTexture *pTexture);
extern int16_t bitmap_mipmap_get_width(const bitmap_data *bitmap, int16_t mipmap_index);
extern int16_t bitmap_mipmap_get_height(const bitmap_data *bitmap, int16_t mipmap_index);
extern int16_t bitmap_format_get_bits_per_pixel(int16_t format);
extern void DecodeBlockRGB__single_pixel(const S3TCBlockRGB *pblockSrc, S3TC_COLOR *colorDst, int16_t u, int16_t v);
extern void DecodeBlockAlpha4__single_pixel(const S3TCBlockAlpha4 *pblockSrc, S3TC_COLOR *colorDst, int16_t u, int16_t v);
extern void DecodeBlockAlpha3__single_pixel(const S3TCBlockAlpha3 *pblockSrc, S3TC_COLOR *colorDst, int16_t u, int16_t v);

unsigned int _rasterizer_bitmap_2d_get_pixel(const bitmap_data *bitmap, const real_point2d *point, float lod)
{
    if (debug_no_drawing)
        return -1;

    void *hardware_format = bitmap->hardware_format;
    if (!hardware_format)
        return -1;

    D3DBaseTexture *hw_texture = hcex_tex_gethw(hardware_format, 0);
    D3DTexture *texture = (D3DTexture *)hw_texture;

    __int16 mip_level;
    if (lod >= 1.0f || bitmap->mipmap_count <= 0)
        mip_level = 0;
    else
        mip_level = (__int16)(int)((1.0f - lod) * (float)bitmap->mipmap_count);

    unsigned int level_count = D3DBaseTexture_GetLevelCount(hw_texture);
    if ((unsigned int)mip_level >= level_count - 1)
        mip_level = (__int16)(level_count - 1);

    __int16 width = bitmap_mipmap_get_width(bitmap, mip_level);
    __int16 height = bitmap_mipmap_get_height(bitmap, mip_level);

    int raw_u = (int)((float)width * point->n[0] - 0.5f);
    __int16 u = ((width - 1) & width) ? (__int16)(((raw_u % width) + width) % width) : (__int16)(raw_u & (width - 1));

    int raw_v = (int)((float)height * point->n[1] - 0.5f);
    __int16 v = ((height - 1) & height) ? (__int16)(((raw_v % height) + height) % height) : (__int16)(raw_v & (height - 1));

    _D3DLOCKED_RECT locked_rect;
    D3DTexture_LockRect(texture, mip_level, &locked_rect, 0, 0x10u);
    char *pBits = (char *)locked_rect.pBits;
    unsigned int pixel;
    if (!locked_rect.pBits)
        return pixel; /* FAITHFUL QUIRK: genuinely uninitialized on this path */

    if (bitmap->flags & (1u << _bitmap_compressed_bit))
    {
        /* compressed (S3TC) bitmap */
        __int16 bits_per_pixel = bitmap_format_get_bits_per_pixel(bitmap->format);
        __int16 block_u = u & 3;
        __int16 block_v = v & 3;
        __int16 block_size = (__int16)((16 * bits_per_pixel) >> 3);
        const void *block = &pBits[block_size * (v / 4 * width / 4 + u / 4)];

        switch ((unsigned __int16)bitmap->format)
        {
            case _bitmap_format_dxt1:
                DecodeBlockRGB__single_pixel((const S3TCBlockRGB *)block, (S3TC_COLOR *)&pixel, block_u, block_v);
                break;
            case _bitmap_format_dxt3:
                DecodeBlockAlpha4__single_pixel((const S3TCBlockAlpha4 *)block, (S3TC_COLOR *)&pixel, block_u, block_v);
                break;
            case _bitmap_format_dxt5:
                DecodeBlockAlpha3__single_pixel((const S3TCBlockAlpha3 *)block, (S3TC_COLOR *)&pixel, block_u, block_v);
                break;
        }

        pixel = (unsigned __int16)((((unsigned __int16)(pixel >> 16)) | (pixel & 0xFFFF0000)) >> 8)
              | (((pixel << 16) | (unsigned __int16)pixel) << 8) & 0xFFFF0000;
        D3DTexture_UnlockRect(texture, mip_level);
        return pixel;
    }

    /* raw (uncompressed) bitmap */
    __int16 raw_format = (unsigned __int16)bitmap->format;
    int row_offset = v * locked_rect.Pitch;

    /* Collapse formats that share a decode path onto their 32-bit-expanded equivalents. */
    if (raw_format == _bitmap_format_r5g6b5)
        raw_format = _bitmap_format_x8r8g8b8;
    if (raw_format == _bitmap_format_a1r5g5b5)
        raw_format = _bitmap_format_a8r8g8b8;
    if (raw_format == _bitmap_format_a4r4g4b4)
        raw_format = _bitmap_format_a8r8g8b8;
    int format_index = raw_format - _bitmap_format_r5g6b5;

    if ((unsigned int)format_index > 5 || format_index == 1)
    {
        D3DTexture_UnlockRect(texture, mip_level);
        return -1;
    }

    switch (format_index)
    {
        case _bitmap_format_a1r5g5b5 - _bitmap_format_r5g6b5: /* 2: A1R5G5B5 */
        {
            unsigned int p = *(unsigned __int16 *)(pBits + row_offset + 2 * u);
            pixel = (2 * ((4 * ((2 * ((4 * ((8 * p) & 0x3E000 | p & 0x3E0)) | p & 0x7000)) | p & 0x1F))
                          | p & 0x380))
                  | (p >> 2) & 7
                  | (-16777216 * (p >> 15));
            D3DTexture_UnlockRect(texture, mip_level);
            return pixel;
        }
        case _bitmap_format_a4r4g4b4 - _bitmap_format_r5g6b5: /* 3: A4R4G4B4 */
        {
            unsigned int p = *(unsigned __int16 *)(pBits + row_offset + 2 * u);
            pixel = (16 * ((16 * ((16 * ((16 * p) & 0xFFFF0000
                                          | p & 0xFFFFF000
                                          | (16 * ((p >> 4) & 0xF0 | (p >> 8) & 0xF))
                                          | ((unsigned char)p >> 4)))
                                  | ((unsigned char)p >> 4)))
                          | p & 0xF))
                  | p & 0xF;
            D3DTexture_UnlockRect(texture, mip_level);
            return pixel;
        }
        case _bitmap_format_x8r8g8b8 - _bitmap_format_r5g6b5: /* 4: already-32-bit (X8R8G8B8 / A8R8G8B8) */
            pixel = *(unsigned int *)(pBits + row_offset + 4 * u);
            D3DTexture_UnlockRect(texture, mip_level);
            return pixel;
        default:
            if (format_index)
            {
                /* 8bpp; FAITHFUL QUIRK: strided by 4*u, matching the 32-bit path, not 1*u */
                pixel = *(unsigned __int8 *)(pBits + row_offset + 4 * u);
            }
            else
            {
                /* R5G6B5 */
                unsigned int p = *(unsigned __int16 *)(pBits + row_offset + 2 * u);
                pixel = (8 * (((32 * (p & 0xFFFFF8FF)) | p) & 0xFFFFE01F | (4 * p) & 0x1F80 | 0xFFE00000))
                      | (((p >> 1) & 0xE | p & 0xFFF1) >> 1) & 0x307;
            }
            D3DTexture_UnlockRect(texture, mip_level);
            return pixel;
    }
}
