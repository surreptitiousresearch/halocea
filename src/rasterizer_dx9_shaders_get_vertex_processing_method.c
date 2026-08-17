/* rasterizer_dx9_shaders_get_vertex_processing_method @0x83724000 */
#include "headers/rasterizer_dx9_shader_tables.h"

unsigned int rasterizer_dx9_shaders_get_vertex_processing_method(unsigned int index)
{
    return vsd_table[index].vertex_processing_method;
}
