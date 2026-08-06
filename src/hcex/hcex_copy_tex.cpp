/* hcex_copy_tex @ 0x823EFFB0
   Copies a texture face into the pct picture buffer. When no expansion is
   needed (block_halving == -1) it is a straight memcpy; otherwise each source
   texel is expanded to A8R8G8B8 (with endian byte-reordering before and after).
   Strong asserts guard the source/destination size relationship and alignment. */

#include <stdint.h>
#include "../headers/hcex/hcex_texture_upload.h"

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern "C" void STRONG_ASSERT_DUMMY_Crash(void *ctx, const char *expr, const char *file, int line, const char *info);
extern "C" void *memcpy(void *dst, const void *src, unsigned int size);
extern void ds_ByteReorder(unsigned char *data, int count, int element_size);
extern "C" unsigned int bitmap_format_to_a8r8g8b8(int16_t format, void *mipmap_address, int pixel_index);

void hcex_copy_tex(unsigned int *pctData, short *src, short block_halving, int src_size, int dst_size)
{
    if ( block_halving == -1 )
    {
        if ( !IGNORE_STRONG_ASSERT && src_size && src_size < dst_size )
            STRONG_ASSERT_DUMMY_Crash(0, "src_size == 0 || src_size >= dst_size",
                                     "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 523, empty_string);
        memcpy(pctData, src, dst_size);
        return;
    }

    if ( !IGNORE_STRONG_ASSERT && src_size && 2 * src_size < dst_size )
        STRONG_ASSERT_DUMMY_Crash(0, "src_size == 0 || src_size * 2 >= dst_size",
                                 "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 525, empty_string);
    if ( !IGNORE_STRONG_ASSERT && dst_size % 4 )
        STRONG_ASSERT_DUMMY_Crash(0, "dst_size % 4 == 0",
                                 "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 531, empty_string);

    ds_ByteReorder((unsigned char *)src, dst_size / 2, 2);
    unsigned int *dst = pctData;
    for ( int i = dst_size / 4; i > 0; --i )
        *dst++ = bitmap_format_to_a8r8g8b8(block_halving, &src, 0);
    ds_ByteReorder((unsigned char *)pctData, dst_size, 4);
}
