#pragma once
// HCEX bridge: a stateless "run this once at static-init time" marker — its ctor/dtor bootstrap
// and tear down a grab-bag of independent one-time global subsystems (input force-feedback
// default, D3D driver factory hookup, the HALO sound system, the physical-memory pool, the UI
// thread policy flag, the per-frame callback extension hook, and the loading-screen system).
// DB-verified size (types_members HCEX_INIT_STAT) — 1 (empty class; all state lives in the
// globals it touches, listed as externs at each call site).

namespace snd { struct SYSTEM_CUSTOM; } // snd subsystem: custom sound backend — boundary, only its
                                          // deleting-dtor vtable slot is modeled (needed by ~HCEX_INIT_STAT)
struct HALO_SOUND_SYSTEM;                // Blam-bridge sound backend implementing snd::SYSTEM_CUSTOM — boundary
struct d3dDRIVER;                        // ws-engine rend: D3D driver instance — boundary

namespace snd { enum INIT; } // snd init-mode enum — boundary
template<class T> struct dsTSTRING;

// Full DB vtable (types_members snd::SYSTEM_CUSTOM_vtbl): the deleting dtor is the LAST
// slot (0x18), after the 6 backend entry points.
struct snd_SYSTEM_CUSTOM_vtbl {
    bool (*Init)(snd::SYSTEM_CUSTOM *self, snd::INIT initMode);            // 0x00
    void (*Term)(snd::SYSTEM_CUSTOM *self);                                // 0x04
    void (*Update)(snd::SYSTEM_CUSTOM *self);                              // 0x08
    void (*InitLevel)(snd::SYSTEM_CUSTOM *self, const dsTSTRING<char> *levelName); // 0x0C
    void (*TermLevel)(snd::SYSTEM_CUSTOM *self);                           // 0x10
    void (*DbgRenderSounds)(snd::SYSTEM_CUSTOM *self);                     // 0x14
    void (*dtr_SYSTEM_CUSTOM)(snd::SYSTEM_CUSTOM *self, int freeMemory);   // 0x18 deleting dtor
};

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
