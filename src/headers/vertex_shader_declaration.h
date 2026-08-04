#pragma once

#include "d3d_shader_boundary.h"

typedef struct vertex_shader_declaration
{
    D3DVertexDeclaration *decl;             /* 0x00 */
    unsigned int fvf;                       /* 0x04 */
    unsigned int vertex_processing_method;  /* 0x08 */
} vertex_shader_declaration; /* 12 bytes */
