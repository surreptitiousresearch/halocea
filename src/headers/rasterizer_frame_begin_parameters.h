#pragma once
#include <stdint.h>
/* rasterizer_frame_begin_parameters — the per-frame timing block handed to rasterizer_frame_begin
 * and mirrored into the global_frame_parameters global. DB layout (PDB ground truth, align(8),
 * sizeof=0x20). */

typedef struct /* align(8) */ rasterizer_frame_begin_parameters
{
    double game_time_sec; /* 0x00 — PPC long double (16 bytes); rasterizer_frame_update stores
                                  this frame's delta into the float at +8, inside this field */
    float       dt;            /* 0x10 */
    char        pad_14[12];    /* 0x14 — trailing align padding to 0x20 */
} rasterizer_frame_begin_parameters;
