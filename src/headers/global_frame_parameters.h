#pragma once
/* global_frame_parameters @0x846DB480 — per-frame rasterizer timing parameters. Its DB type is
 * rasterizer_frame_begin_parameters (game_time_sec long double @0x00, dt float @0x10); see that header. */

#include "rasterizer_frame_begin_parameters.h"

extern rasterizer_frame_begin_parameters global_frame_parameters;
