#pragma once
/* transparent_geometry_group — one queued transparent draw call (a shader + its geometry buffers + sort key).
 * Layout from the database (168 bytes). Upgraded from an earlier byte-placeholder revision now that all
 * embedded sub-struct headers (render_model_effect, triangle_buffer, vertex_buffer, bitmap_data,
 * real_matrix4x3, render_lighting, render_animation) are resolved. */

#include "shader.h"
#include "render_model_effect.h"
#include "real_vector2d.h"
#include "triangle_buffer.h"
#include "vertex_buffer.h"
#include "bitmap_data.h"
#include "real_matrix4x3.h"
#include "render_lighting.h"
#include "render_animation.h"
#include "real_point3d.h"
#include "real_plane3d.h"

typedef struct transparent_geometry_group
{
    unsigned int             geometry_flags;                                    /* 0x00 */
    int                      object_index;                                      /* 0x04 */
    int                      source_object_index;                               /* 0x08 */
    const shader            *shader;                                            /* 0x0C */
    __int16                  shader_permutation_index;                         /* 0x10 */
    unsigned char            _pad12[2];                                         /* 0x12 */
    render_model_effect      effect;                                            /* 0x14 (40) */
    real_vector2d            model_base_map_scale;                              /* 0x3C */
    int                      dynamic_triangle_buffer_index;                     /* 0x44 */
    union                                                                       /* 0x48 — tagged by `shader`: */
    {
        const triangle_buffer *triangle_buffer;                                 /*   shader != NULL: geometry to draw */
        void                 (*render_proc)(int, int);                          /*   shader == NULL (widget path): callback, called with (first_triangle_index, triangle_count) */
    };
    int                      first_triangle_index;                              /* 0x4C (widget path: render_proc arg 1) */
    int                      triangle_count;                                    /* 0x50 (widget path: render_proc arg 2) */
    int                      dynamic_vertex_buffer_index;                       /* 0x54 */
    const vertex_buffer     *vertex_buffers;                                    /* 0x58 */
    const bitmap_data       *lightmap;                                          /* 0x5C */
    const real_matrix4x3    *node_matrices;                                     /* 0x60 */
    __int16                  node_matrix_count;                                 /* 0x64 */
    unsigned char            _pad66[2];                                         /* 0x66 */
    const unsigned __int8   *local_node_remap_table;                            /* 0x68 */
    unsigned int             local_node_remap_table_size;                       /* 0x6C */
    const render_lighting   *lighting;                                          /* 0x70 */
    const render_animation  *animation;                                         /* 0x74 */
    float                    z_sort;                                            /* 0x78 */
    real_point3d             centroid;                                          /* 0x7C */
    real_plane3d             plane;                                             /* 0x88 */
    int                      sorted_index;                                      /* 0x98 */
    __int16                  prev_group_presorted_index;                        /* 0x9C */
    __int16                  next_group_presorted_index;                        /* 0x9E */
    int                      active_camouflage_transparent_source_object_index; /* 0xA0 */
    unsigned __int8          sort_last;                                         /* 0xA4 */
    unsigned __int8          cortana_hack;                                      /* 0xA5 */
    unsigned char            _pad_end[2];                                       /* 0xA6 — trailing struct pad */
} transparent_geometry_group;                                                   /* 168 bytes */
