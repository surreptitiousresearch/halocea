/* hcex_calc_tex_hw_size @0x823F0298 — compute the hardware memory footprint of a Halo bitmap once uploaded.
 * Converts the D3D/Halo format to a pct picture format, fills a pctHEADER from the bitmap dimensions
 * (bm_type 1 = volume -> depth; bm_type 2 = cube -> 6 faces), and for block-compressed formats clamps the
 * effective mip count so no mip smaller than a 4x4 block is included, then returns pctGetMemSize. pct* is a
 * picture-format boundary; hcex_conv_tex_fmt is a bridge sibling. */

#include "../headers/pct_boundary.h"          /* pctHEADER, pctGetMemSize, pctIsCompressed */
#include "../headers/hcex/hcex_texture_upload.h" /* hcex_conv_tex_fmt, _D3DFORMAT */
#include "../headers/bitmap_type.h"           /* bitmap_type_3d, bitmap_type_cube_map */

extern "C" int hcex_calc_tex_hw_size(int width, int height, int mipmaps, int depth, _D3DFORMAT fmt, int bm_type)
{
    int   pct_format = -1;
    short block_halving = -1;
    hcex_conv_tex_fmt(fmt, &pct_format, &block_halving);

    pctHEADER header;
    header.sx = width;
    header.sy = height;
    header.nFaces = 1;
    header.sign = 0;
    header.sz = 1;
    header.nMipMap = mipmaps;
    header.format = pct_format;

    if ( bm_type == bitmap_type_3d )
        header.sz = depth;
    else if ( bm_type == bitmap_type_cube_map )
        header.nFaces = 6;

    if ( pctIsCompressed(pct_format) )
    {
        int mip = mipmaps - 1;
        int min_dim = width >> (mipmaps - 1);
        if ( min_dim >= height >> (mipmaps - 1) )
            min_dim = height >> (mipmaps - 1);

        /* drop mips whose smaller dimension is below one 4x4 compression block */
        if ( min_dim < 4 )
        {
            do
            {
                if ( mip <= 0 )
                    break;
                --mip;
                min_dim = width >> mip;
                if ( width >> mip >= height >> mip )
                    min_dim = height >> mip;
            }
            while ( min_dim < 4 );
        }
        header.nMipMap = mip + 1;
    }

    return pctGetMemSize(&header);
}
