#pragma once
/* gbxmodel_geometry_part — one renderable part of a model geometry (structure.c / model.c). Layout
 * from the database (132 bytes, via types_members). */

#include "tag_block.h"
#include "real_point3d.h"
#include "triangle_buffer.h"
#include "vertex_buffer.h"

typedef struct gbxmodel_geometry_part
{
    unsigned int    flags;                          /* 0x00 */
    __int16         shader_index;                   /* 0x04 */
    char            prev_part_index;                /* 0x06 */
    char            next_part_index;                /* 0x07 */
    __int16         centroid_primary_node_index;     /* 0x08 */
    __int16         centroid_secondary_node_index;   /* 0x0A */
    float           centroid_primary_node_weight;    /* 0x0C */
    float           centroid_secondary_node_weight;  /* 0x10 */
    real_point3d    centroid;                        /* 0x14 */
    tag_block       uncompressed_vertices;           /* 0x20 */
    tag_block       compressed_vertices;             /* 0x2C */
    tag_block       triangles;                       /* 0x38 */
    triangle_buffer triangle_buffer;                 /* 0x44 */
    vertex_buffer   vertex_buffer;                   /* 0x54 */
    unsigned __int8 padA;                            /* 0x68 */
    unsigned __int8 padB;                            /* 0x69 */
    unsigned __int8 padC;                             /* 0x6A */
    unsigned __int8 num_nodes;                        /* 0x6B */
    unsigned __int8 local_node_table[24];              /* 0x6C */
} gbxmodel_geometry_part;                                /* 0x84 (132 bytes) */
