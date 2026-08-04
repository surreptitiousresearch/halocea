#pragma once
// Stack-built render-pass descriptor consumed by aiDEBUG::RenderDebugInfoMesh (a8_debug.cpp). The
// concrete vid render-pass descriptor type is a boundary (next frontier); the layout below is a
// faithful transcription of the DB decompile's stack writes at 0x83194690 — field offsets are
// exact (0x20 bytes), field *semantics* are inferred from the constants written (RGB colour scale,
// alpha, per-feature flags, optional texture).
struct txmTEXTURE;

struct aiDBG_MESH_PASS_DESC {
    unsigned char enable;         // 0x00 = 1
    unsigned char flag01;         // 0x01 = 0
    unsigned char _pad02[2];      // 0x02
    float         colorScaleR;    // 0x04 = 1.0
    float         colorScaleG;    // 0x08 = 1.0
    float         colorScaleB;    // 0x0C = 1.0
    float         alpha;          // 0x10 = caller alpha
    unsigned char flag14;         // 0x14 = 1
    unsigned char flag15;         // 0x15 = 0
    unsigned char flag16;         // 0x16 = 0
    unsigned char zTest;          // 0x17 = caller zTest
    unsigned char flag18;         // 0x18 = 1
    unsigned char flag19;         // 0x19 = 0
    unsigned char flag1A;         // 0x1A = 0
    unsigned char _pad1B;         // 0x1B
    txmTEXTURE   *texture;        // 0x1C = null (unused by the debug mesh path)
};
