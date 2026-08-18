/* hcex_tex_cube_copytohw @ 0x823F0AF8
   Cubemap variant: creates a 6-face pct picture and copies each face's mip
   chain (remapped through face_mapping_table). Sets the cube flag (tex[30] |=
   0x800) on the texture object before handing over the picture.

   Deviation: the binary bounds the 6-face inner loop with the address of an
   adjacent string literal ("tex_size != 0"); reproduced as a fixed 6-face loop.
   The block-halving branch (when the format is half-block) halves both the copy
   size and the source offset, matching the binary. */

#include "../headers/pct_boundary.h"
#include "../headers/hcex/hcex_texture_upload.h"

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h" // the only Crash is a C++ member, not a C symbol
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

extern "C" void hcex_tex_cube_copytohw(unsigned int *tex, char *tex_data, int width, int height,
                            int mipmaps, _D3DFORMAT fmt, int tex_size)
{
    int pct_format = -1;
    short block_halving = -1;
    hcex_conv_tex_fmt(fmt, &pct_format, &block_halving);

    pctPICTURE *pic = pctCreate(pct_format, width, height, mipmaps, 6, 1);
    pctGetMemFaceSize(&pic->hdr);

    if ( !IGNORE_STRONG_ASSERT && !tex_size )
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("tex_size != 0",
                                 "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 634, empty_string);

    for ( int mipmap = 0; mipmap < pic->hdr.nMipMap; ++mipmap )
    {
        for ( int face = 0; face < 6; ++face )
        {
            int mip_mem_size = pctGetMipMapMemSize(&pic->hdr, mipmap);
            int full_mem_size = pctGetMipMapMemSize(&pic->hdr, mipmap);
            unsigned int *dst = (unsigned int *)pctGetMipMapOffsetPtr(pic, mipmap, face_mapping_table[face]);
            int src_offset = 6 * pctGetMipMapOffset(&pic->hdr, mipmap) + mip_mem_size * face;
            if ( block_halving != -1 )
            {
                mip_mem_size /= 2;
                src_offset /= 2;
            }
            hcex_copy_tex(dst, (short *)&tex_data[src_offset], block_halving, mip_mem_size, full_mem_size);
        }
    }

    tex[30] |= 0x800u; /* txmTEXTURE::state @0x78: cube flag */
    hcex_halo_tex_size -= tex[28] /* txmTEXTURE::texSize @0x70 */;
    (*(void (**)(unsigned int *, pctPICTURE *, int))(*tex + 76))(tex, pic, 0);
    hcex_halo_tex_size += tex[28];

    pctGetFormat(pic); /* DEVIATION: bl pctGetFormat(pctPICTURE *) @0x823F0CBC -> the pctPICTURE* overload */
    pctDestroy(pic);
}
