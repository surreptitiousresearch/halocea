/* hcex_tex_2d_copytohw @ 0x823F0478
   Uploads a 2D bitmap's pixels into the engine's hardware texture (txmTEXTURE):
   builds a pct picture in the target hardware format, copies the source texels
   into its mip chain, then hands the picture to the texture object (vtable +76)
   while keeping the Halo texture-memory accounting (txmTEXTURE::texSize, tex[28]) in sync. */

#include "../headers/pct_boundary.h"
#include "../headers/hcex/hcex_texture_upload.h"

extern "C" void hcex_tex_2d_copytohw(unsigned int *tex, short *tex_data, int width, int height,
                          int mipmaps, _D3DFORMAT fmt, int tex_size)
{
    int pct_format = -1;
    short block_halving = -1;
    hcex_conv_tex_fmt(fmt, &pct_format, &block_halving);

    pctPICTURE *pic = pctCreate(pct_format, width, height, mipmaps, 1, 1);
    unsigned int *dst = (unsigned int *)pctGetMipMapOffsetPtr(pic, 0, 0);
    int face_size = pctGetMemFaceSize(&pic->hdr);
    hcex_copy_tex(dst, tex_data, block_halving, tex_size, face_size);

    hcex_halo_tex_size -= tex[28]; /* txmTEXTURE::texSize @0x70 */
    /* txmTEXTURE method (vtable +76): install the picture as hardware data */
    (*(void (**)(unsigned int *, pctPICTURE *, int))(*tex + 76))(tex, pic, 0);
    hcex_halo_tex_size += tex[28];

    pctGetFormat(pic); /* DEVIATION: bl pctGetFormat(pctPICTURE *) @0x823F054C -> the pctPICTURE* overload */
    pctDestroy(pic);
}
