#pragma once

#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_plane3d.h"
#include "rectangle2d.h"

/* 84-byte render camera. */
typedef struct render_camera
{
    real_point3d position;        // 0x00
    real_vector3d forward;        // 0x0C
    real_vector3d up;             // 0x18
    unsigned char mirrored;       // 0x24
    unsigned char _pad0[3]; /* db-verified padding */
    float vertical_field_of_view; // 0x28
    rectangle2d viewport_bounds;  // 0x2C
    rectangle2d window_bounds;    // 0x34
    float z_near;                 // 0x3C
    float z_far;                  // 0x40
    real_plane3d mirror_plane;    // 0x44
} render_camera;
