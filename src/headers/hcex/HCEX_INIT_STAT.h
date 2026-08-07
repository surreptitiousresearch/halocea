#pragma once
// HCEX bridge: a stateless "run this once at static-init time" marker — its ctor/dtor bootstrap
// and tear down a grab-bag of independent one-time global subsystems (input force-feedback
// default, D3D driver factory hookup, the HALO sound system, the physical-memory pool, the UI
// thread policy flag, the per-frame callback extension hook, and the loading-screen system).
// DB-verified size (types_members HCEX_INIT_STAT) — 1 (empty class; all state lives in the
// globals it touches, listed as externs at each call site).

// DEVIATION: this header carried a third copy of snd::SYSTEM_CUSTOM_vtbl (as `snd_SYSTEM_CUSTOM_vtbl`)
// that declared `namespace snd { enum INIT; }`. The DB (types_members snd::INIT) says INIT is a
// STRUCT of 8 bytes (waveOutput/enableEax/enableHW/hrtf), not an enum, and slot 0 takes it BY VALUE.
// SYSTEM_CUSTOM.h is the sole home for snd::HRTF / snd::INIT / snd::SYSTEM_CUSTOM / SYSTEM_CUSTOM_vtbl.
#include "../ws/snd/SYSTEM_CUSTOM.h"
struct HALO_SOUND_SYSTEM;                // Blam-bridge sound backend implementing snd::SYSTEM_CUSTOM — boundary
struct d3dDRIVER;                        // ws-engine rend: D3D driver instance — boundary

typedef struct HCEX_INIT_STAT {
    // 0x823E3B4C — clear the force-feedback default, install CrateHaloD3dDrv as the D3D driver
    // factory, heap-allocate and construct the HALO_SOUND_SYSTEM as the active custom sound
    // backend (falling back to null on allocation failure), reserve the physical-memory pool,
    // force the UI off the dedicated UI thread, install hcexCallFrame as the per-frame callback
    // extension, and initialize the loading-screen system.
    HCEX_INIT_STAT();

    // 0x823CEB94 — if a custom sound backend is installed, invoke its deleting destructor
    // (freeMemory=1) and clear the global pointer.
    ~HCEX_INIT_STAT();
} HCEX_INIT_STAT;
