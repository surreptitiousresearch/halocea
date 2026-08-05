#pragma once
/* rasterizer_globals_struct — global rasterizer state and configuration (96 bytes). current_lock_operation
 * marks the active dynamic-geometry lock so nested draws can be validated. Layout from the database. */

#include <stdint.h>
#include "rectangle2d.h"

typedef struct rasterizer_globals_struct
{
    uint8_t  active;                                  /* 0x00 */
    unsigned char _pad0[1]; /* db-verified padding */
    int16_t          current_lock_operation;                  /* 0x02 */
    rectangle2d      screen_bounds;                           /* 0x04 */
    rectangle2d      frame_bounds;                            /* 0x0C */
    unsigned char _pad1[4]; /* db-verified padding */
    int64_t          frame_index;                             /* 0x18 */
    unsigned int     flip_index;                              /* 0x20 */
    uint8_t  use_floating_point_zbuffer;              /* 0x24 */
    uint8_t  use_rasterizer_frame_rate_throttle;      /* 0x25 */
    uint8_t  use_rasterizer_frame_rate_stabilization; /* 0x26 */
    unsigned char _pad2[1]; /* db-verified padding */
    int16_t          refresh_rate;                            /* 0x28 */
    unsigned char _pad3[2]; /* db-verified padding */
    float            z_near;                                  /* 0x2C */
    float            z_far;                                   /* 0x30 */
    float            z_near_first_person;                     /* 0x34 */
    float            z_far_first_person;                      /* 0x38 */
    void            *default_white_hardware_format;           /* 0x3C */
    void            *default_2d_hardware_format;              /* 0x40 */
    void            *default_3d_hardware_format;              /* 0x44 */
    void            *default_cm_hardware_format;              /* 0x48 */
    int16_t          lightmap_mode;                           /* 0x4C */
    int16_t          maximum_nodes_per_model;                 /* 0x4E */
    int16_t          max_simultaneous_textures;               /* 0x50 */
    unsigned char _pad4[2]; /* db-verified padding */
    unsigned int     fixed_function_ambient;                  /* 0x54 */
    uint8_t  use_cheap_active_camo;                   /* 0x58 */
    uint8_t  render_targets_disabled;                 /* 0x59 */
    uint8_t  alpha_render_targets_disabled;           /* 0x5A */
} rasterizer_globals_struct;                                  /* 96 bytes */
