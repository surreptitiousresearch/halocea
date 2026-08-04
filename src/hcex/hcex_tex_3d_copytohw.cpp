/* hcex_tex_3d_copytohw @ 0x823F0388
   Volume-texture variant of hcex_tex_2d_copytohw: the pct picture is created
   with `depth` slices. */

#include "../headers/pct_boundary.h"
#include "../headers/hcex/hcex_texture_upload.h"

extern "C" void hcex_tex_3d_copytohw(unsigned int *tex, short *tex_data, int width, int height,
                          int mipmaps, int depth, _D3DFORMAT fmt, int tex_size)
{
    int pct_format = -1;
    short block_halving = -1;
    hcex_conv_tex_fmt(fmt, &pct_format, &block_halving);

    pctPICTURE *pic = pctCreate(pct_format, width, height, mipmaps, 1, depth);
    unsigned int *dst = (unsigned int *)pctGetMipMapOffsetPtr(pic, 0, 0);
    int face_size = pctGetMemFaceSize(&pic->hdr);
    hcex_copy_tex(dst, tex_data, block_halving, tex_size, face_size);

    hcex_halo_tex_size -= tex[28] /* txmTEXTURE::texSize @0x70 */;
    (*(void (**)(unsigned int *, pctPICTURE *, int))(*tex + 76))(tex, pic, 0);
    hcex_halo_tex_size += tex[28];

    pctGetFormat(pic->hdr.format); /* decl takes format code, not picture */
    pctDestroy(pic);
}
