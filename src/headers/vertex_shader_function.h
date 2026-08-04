#pragma once

#include "d3d_shader_boundary.h"

typedef struct vertex_shader_function
{
    D3DVertexShader *shader; /* 0x00 */
    char *filename;          /* 0x04 */
} vertex_shader_function; /* 8 bytes */
