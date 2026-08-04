#pragma once
// DB-verified via types_members _C0B17834CF57C3AF506ACEE9F838F6EA (anonymous PDB type)

#include "dynamic_vertex_buffer_group.h"
#include "dynamic_vertex_buffer.h"

typedef struct dynamic_vertices_struct
{
    dynamic_vertex_buffer_group groups[18]; // 0x0000
    dynamic_vertex_buffer buffers[1024];    // 0x00D8  (opaque here)
    int buffer_count;                       // 0x50D8
} dynamic_vertices_struct;

extern dynamic_vertices_struct dynamic_vertices;
