#include "headers/rasterizer_dx9_shader_tables.h"

unsigned int rasterizer_dx9_shaders_get_fvf(unsigned int index)
{
    return vsd_table[index].fvf;
}
