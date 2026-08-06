#pragma once

#include <stdint.h>
#include "rasterizer_dx9_shader_table.h"

#ifdef __cplusplus
extern "C" {
#endif
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern unsigned int rasterizer_shader_technique_for_name(const char *name, ID3DXEffect *effect);
#ifdef __cplusplus
}
#endif
