#pragma once
/* structure_material — one material (shader + geometry) chunk of a structure BSP. */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_data.h"
#include "real_point3d.h"
#include "real_plane3d.h"
#include "render_lighting.h"
#include "vertex_buffer.h"

typedef struct structure_material
{
    tag_reference   shader;                  /* 0x00 */
    int16_t         permutation_index;       /* 0x10 */
    unsigned short  flags;                   /* 0x12 */
    int             first_surface_index;     /* 0x14 */
    int             surface_count;           /* 0x18 */
    real_point3d    centroid;                /* 0x1C */
    render_lighting lighting;                /* 0x28 */
    real_plane3d    plane;                   /* 0x9C */
    int16_t         breakable_surface_index; /* 0xAC */
    unsigned short  copious_unused_space;    /* 0xAE */
    vertex_buffer   vertices;                /* 0xB0 */
    vertex_buffer   lightmap_vertices;       /* 0xC4 */
    tag_data        uncompressed_vertex_data;/* 0xD8 */
    tag_data        compressed_vertex_data;  /* 0xEC */
} structure_material;                        /* 256 bytes */
