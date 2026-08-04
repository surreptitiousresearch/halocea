#pragma once
// ws-engine mdl: per-model collision-detection tuning block (radii, heights, physics weights).
// DB-verified layout (headers_ref mdlCDT_SETTINGS) — size 0x24.

struct mdlCDT_SETTINGS {
    float radCdt;          // 0x00 base collision radius
    float radCdtAnim;      // 0x04 anim-cylinder collision radius
    float heightCdtAnim;   // 0x08 anim-cylinder height
    float radCdtSafe;      // 0x0C safe (padded) collision radius
    bool  checkFloorCrash; // 0x10
    float charPhysWeight;  // 0x14 character physics weight
    float wpnPointDistMin; // 0x18 minimum weapon-point distance
    bool  neverSkipBodies; // 0x1C
    float distSkipFloor;   // 0x20 distance below which the floor test is skipped
};
