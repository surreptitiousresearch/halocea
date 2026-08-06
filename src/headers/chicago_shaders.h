#pragma once

#include <stdint.h>
#include "d3d_shader_boundary.h"  /* D3DVertexShader etc. */
#include "shader_stage_cfg.h"
#include "chicago_pixel_shader.h"  /* provides D3DPixelShader */

#ifdef __cplusplus
extern "C" {
#endif

extern void *shader_bin_handle;               /* open blob handle during dump */
extern chicago_pixel_shader chicago_shader_table[1014]; /* DB applied type: [1014] (was misdeclared [2]) */
extern int chicago_shader_table_size;
extern D3DPixelShader *decal_shader; /* DB applied type (was void*) */

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void chicago_fill_const(int *bool_const, shader_stage_cfg stage);
extern void set_chicago_shader(int is_cube, shader_stage_cfg s1, shader_stage_cfg s2, shader_stage_cfg s3, int s4_arg, int s4_op);
extern void set_decal_shader(int16_t local_framebuffer_blend_function);

extern D3DPixelShader *D3DDevice_CreatePixelShader(const unsigned int *function);
extern void D3DDevice_SetPixelShader(struct D3DDevice *device, D3DPixelShader *shader);
extern void D3DDevice_SetPixelShaderConstantB(struct D3DDevice *device, unsigned int reg,
                                              const int *data, unsigned int count);
#ifdef __cplusplus
}
#endif
