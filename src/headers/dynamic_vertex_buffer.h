#pragma once

typedef struct dynamic_vertex_buffer
{
    short type;               /* 0x00 */
    short _pad02;             /* 0x02 */
    int   vertex_start_index; /* 0x04 */
    int   vertex_buf_offset;  /* 0x08 */
    int   vertex_count;       /* 0x0C */
    void *data;               /* 0x10 */
} dynamic_vertex_buffer;      /* 20 bytes */
