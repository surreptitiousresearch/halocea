#pragma once
#include "real_point3d.h"

typedef struct path_input
{
    float           pathfinding_radius;         /* 0x00 */
    unsigned char   ignore_broken_surfaces;     /* 0x04 */
    unsigned char   _pad05[3];
    int             ignore_source_object_index; /* 0x08 */
    int             ignore_target_object_index; /* 0x0C */
    unsigned char   start_valid;                /* 0x10 */
    unsigned char   _pad11[3];
    real_point3d    start_point;                /* 0x14 */
    int             start_surface_index;        /* 0x20 */
    unsigned char   attractor_valid;            /* 0x24 */
    unsigned char   _pad25[3];
    real_point3d    attractor_point;            /* 0x28 */
    int             attractor_object_index;     /* 0x34 */
    float           attractor_radius;           /* 0x38 */
    float           attractor_weight;           /* 0x3C */
    unsigned char   search_bounded;             /* 0x40 */
    unsigned char   _pad41[3];
    float           search_maximum_distance;    /* 0x44 */
} path_input;                                   /* 72 bytes */
