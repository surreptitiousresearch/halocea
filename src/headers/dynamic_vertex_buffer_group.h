#pragma once

typedef struct dynamic_vertex_buffer_group
{
    int          vertex_count;        /* 0x00 */
    int          max_vertex_count;    /* 0x04 */
    unsigned int vertex_buffer_index; /* 0x08 */
} dynamic_vertex_buffer_group;        /* 12 bytes */
