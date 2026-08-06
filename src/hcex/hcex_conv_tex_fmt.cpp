/* hcex_conv_tex_fmt @ 0x823F0178
   Maps a D3D/Halo texture format code to a pct picture format (out_format) and,
   for formats needing CPU expansion, the source Halo bitmap format
   (out_block_halving; -1 means "no expansion, copy as-is"). Unknown formats
   trigger a strong assert. The numeric format codes are the engine's fourcc-
   style identifiers, preserved verbatim. */

#include "../headers/hcex/hcex_texture_upload.h"

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern "C" void STRONG_ASSERT_DUMMY_Crash(void *ctx, const char *expr, const char *file, int line, const char *info);

void hcex_conv_tex_fmt(_D3DFORMAT fmt, int *out_format, short *out_block_halving)
{
    *out_format = -1;
    *out_block_halving = -1;

    /* case labels are _D3DFORMAT enum values (verbatim from DB types_enum_values) */
    switch ( fmt )
    {
        case D3DFMT_A1R5G5B5: *out_format = 0; *out_block_halving = 8; return;  /* 0x18280003 */
        case D3DFMT_A4R4G4B4: *out_format = 0; *out_block_halving = 9; return;  /* 0x1828000F */
        case D3DFMT_A8R8G8B8: *out_format = 0; return;             /* 0x18280046 uncompressed, no expand */
        case D3DFMT_DXT1:     *out_format = 13; return;            /* 0x1A200052 */
        case D3DFMT_DXT3:     *out_format = 15; return;            /* 0x1A200053 (== D3DFMT_DXT2) */
        case D3DFMT_DXT5:     *out_format = 17; return;            /* 0x1A200054 (== D3DFMT_DXT4) */
        case D3DFMT_R5G6B5:   *out_format = 0; *out_block_halving = 6; return;  /* 0x28280004 */
        case D3DFMT_X8R8G8B8: *out_format = 0; return;             /* 0x28280046 */
        default:
            if ( !IGNORE_STRONG_ASSERT )
                STRONG_ASSERT_DUMMY_Crash(0, "0", "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 576, empty_string);
            return;
    }
}
