#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsARRAY_global.h"
#include "../m3d/m3dBOX.h"
// ws-engine anim: a single animation sequence (one layer's keyframed clip).
// DB-verified layout (types_members animSEQ) — size 76 (0x4C).

struct animTRACK;     // anim subsystem — evaluated keyframe track (pointer only)  boundary
struct animBANK_FILE; // anim subsystem — source bank file (pointer only)          boundary

/* DB anonymous union _D960B3461181C14D64416F585E064BE1 (4 bytes) — the clip source
 * ($-name kept verbatim; $ in identifiers is an MSVC extension). */
typedef union _D960B3461181C14D64416F585E064BE1 {
    animTRACK     *track; /* 0x00 */
    animBANK_FILE *bank;  /* 0x00 */
} _D960B3461181C14D64416F585E064BE1;

typedef struct animSEQ {
    struct AF; // action-frame record (element of actionFrames)  boundary

    dsTSTRING<char>    name;         // 0x00
    int                layerId;      // 0x04
    float              startFrame;   // 0x08
    float              endFrame;     // 0x0C
    float              offsetFrame;  // 0x10
    float              lenFrame;     // 0x14
    float              timeSec;      // 0x18 total duration in seconds
    dsARRAY<AF>        actionFrames; // 0x1C
    int                stateSeq;     // 0x2C state flags (0x100 = loaded, 0x200 = reverse-time)
    union _D960B3461181C14D64416F585E064BE1 ___u9; // 0x30 DB anonymous union (track/bank)
    m3dBOX             bbox;         // 0x34

    // 0x8252C3C8 — map a frame index to a clamped time in seconds (const).
    float FrameToTime(float frame) const;
} animSEQ;
