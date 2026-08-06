#pragma once

#include "../d3d_render_boundary.h"  /* _D3DFORMAT */

/* hcex texture-upload helpers and accounting. */
extern int hcex_halo_tex_size;
extern "C" short face_mapping_table[6];

extern void hcex_conv_tex_fmt(_D3DFORMAT fmt, int *out_format, short *out_block_halving);
extern void hcex_copy_tex(unsigned int *dst, const short *src, short block_halving,
                          int src_size, int dst_size);
