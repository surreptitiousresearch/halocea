#pragma once

#include "render_camera.h"
#include "render_frustum.h"
#include "render_fog.h"
#include "render_screen_flash.h"
#include "render_screen_effect.h"

typedef struct rasterizer_window_begin_parameters
{
    short rasterizer_target;       // 0x00
    short window_index;            // 0x02
    unsigned char has_mirror;      // 0x04
    unsigned char suppress_clear;  // 0x05
    unsigned char _pad0[2]; /* db-verified padding */
    /* 0x06..0x07 padding */
    render_camera camera;          // 0x08
    render_frustum frustum;        // 0x5C
    render_fog fog;                // 0x1E8 — 80 bytes
    render_screen_flash screen_flash; // 0x238
    render_screen_effect screen_effect; // 0x250 — 8 bytes
} rasterizer_window_begin_parameters; // 600 bytes

extern rasterizer_window_begin_parameters global_window_parameters;
extern short global_current_render_target;
