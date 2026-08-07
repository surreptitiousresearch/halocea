#pragma once
// ws-engine vid: per-pass environment block (render-block class, light counts, lod, camera
// distance). DB-verified layout (types_members vidPASS_ENV): renderBlock@0 (RENDERBLOCK, 1B),
// lights@4 (LIGHTS, 8), lod@12, camDist@16, state@20 — size 24.

struct vidPASS_ENV {
    // DB-verified values (types_enum_values vidPASS_ENV::RENDERBLOCK). Stored as 1 byte — the
    // fixed underlying type is load-bearing and was recorded ONLY in rendDRIVER.h's forward
    // declaration (`enum RENDERBLOCK : unsigned char`) until 2026-08-07. That declaration spelled
    // vidPASS_ENV as a NAMESPACE, contradicting this struct, and removing it without merging the
    // underlying type here made rendDRIVER::curRenderBlock a 4-byte int and shifted curCamIdx.
    enum RENDERBLOCK : unsigned char {
        RENDERBLOCK_COLOR = 0,
        RENDERBLOCK_DEPTH = 1,
        RENDERBLOCK_SM    = 2,
        RENDERBLOCK_DUDV  = 3,
    };

    // DB-verified (types_members vidPASS_ENV::LIGHTS): nSM@0, nDyn@4 — size 8.
    struct LIGHTS {
        int nSM;  // 0x00 shadow-mapped light count
        int nDyn; // 0x04 dynamic light count
    };

    unsigned char renderBlock; // 0x00 RENDERBLOCK (DB stores as 1 byte)
    unsigned char _pad01[3];   // 0x01
    LIGHTS        lights;      // 0x04
    int           lod;         // 0x0C
    float         camDist;     // 0x10
    unsigned char state;       // 0x14
    unsigned char _pad15[3];   // 0x15
};                             // 24 bytes
