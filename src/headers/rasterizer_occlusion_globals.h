#pragma once

#include "d3d_render_boundary.h"

/* Opaque DX9 shader wrapper; only its address is used as the end sentinel of
   the occlusion_query[] array (the two are adjacent in the data segment). */
typedef struct rasterizer_dx9_shader rasterizer_dx9_shader;

extern unsigned char supports_occlusion_test;
extern D3DQuery *occlusion_query[1024];
extern rasterizer_dx9_shader *widget_dxeffect_shader;
