#pragma once
// ws-engine m3d math: linear-congruential pseudo-random generator (MSVC rand() constants).
// DB-verified layout (types_members m3dRND_GEN): seed@0 (unsigned int) — size 4.

typedef struct m3dRND_GEN {
    unsigned int seed;

    // 0x82561FA0
    int RndInt();

    // 0x82562FF8 (m3dRND_GEN::RndIntMax) — body external to this batch (HALO_SOUND_SYSTEM
    // drain only needed the signature to type call sites). boundary.
    int RndIntMax(int max);

    // Uniform float in [0,1). boundary — body external to this batch.
    float RndFloat();
    // Uniform float in [min,max]. boundary — body external to this batch.
    float RndFloatRange(float min, float max);

    // 0x82563038 (?RndIntRange@m3dRND_GEN@@QAAHHH@Z) — uniform int in [min,max]. boundary.
    int RndIntRange(int min, int max);
} m3dRND_GEN;

// Process-wide shared RNG instance. boundary — defined in the m3d runtime.
extern m3dRND_GEN m3dRndGen;
