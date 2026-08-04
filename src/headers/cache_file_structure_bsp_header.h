#pragma once
/* cache_file_structure_bsp_header — the in-cache header prepended to a loaded structure BSP block.
 * Points at the parsed structure_bsp tag data and records the vertex-buffer block location. */

struct structure_bsp;

typedef struct cache_file_structure_bsp_header
{
    struct structure_bsp *structure_bsp;          /* 0x00 — parsed BSP tag data */
    int                   vertex_buffer_size;     /* 0x04 */
    int                   vertex_buffer_file_ofs; /* 0x08 */
    int                   dummy[2];               /* 0x0C */
    unsigned int          signature;              /* 0x14 */
} cache_file_structure_bsp_header;                /* >= 24 bytes */
