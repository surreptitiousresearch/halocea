#pragma once
#include <stdint.h>
/* rasterizer_frame_begin_parameters — the per-frame timing block handed to rasterizer_frame_begin
 * and mirrored into the global_frame_parameters global. DB layout (PDB ground truth, align(8),
 * sizeof=0x20). */

typedef struct /* align(8) */ rasterizer_frame_begin_parameters
{
    /* 0x00 — one 16-byte PPC `long double` per the DB. MSVC's long double is 8 bytes, so the
     * field is modelled as its leading double plus the remaining 8 bytes. rasterizer_frame_update
     * stores this frame's delta as a float at +8, *inside* this field, and _rasterizer_frame_begin
     * @0x8369BD60 copies the block into global_frame_parameters as two doublewords
     * (`ld r7,0(r31)` / `ld r6,8(r31)`), so all 16 bytes carry data. */
    double      game_time_sec; /* 0x00 */
    char        pad_08[8];     /* 0x08 — tail of the long double slot */
    float       dt;            /* 0x10 */
    char        pad_14[12];    /* 0x14 — trailing align padding to 0x20 */
} rasterizer_frame_begin_parameters;
