#pragma once

#include "real_rectangle2d.h"
#include "real_rectangle3d.h"
#include "real_matrix4x3.h"
#include "real_plane3d.h"
#include "real_point3d.h"
#include "real_vector2d.h"

/* 396-byte view frustum (matrices, planes, projection). */
typedef struct render_frustum
{
    real_rectangle2d frustum_bounds;            // 0x000
    real_matrix4x3 world_to_view;               // 0x010
    real_matrix4x3 view_to_world;               // 0x044
    real_plane3d world_planes[6];               // 0x078
    float z_near;                               // 0x0D8
    float z_far;                                // 0x0DC
    real_point3d world_vertices[5];             // 0x0E0
    real_point3d world_midpoint;                // 0x11C
    real_rectangle3d world_bounds;              // 0x128
    unsigned char projection_valid;             // 0x140
    unsigned char _pad0[3]; /* db-verified padding */
    float projection_matrix[4][4];              // 0x144
    real_vector2d projection_world_to_screen;   // 0x184
} render_frustum;
