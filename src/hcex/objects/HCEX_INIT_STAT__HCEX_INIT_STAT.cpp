/* HCEX_INIT_STAT::HCEX_INIT_STAT @0x823E3B4C — one-time static bootstrap of a grab-bag of
 * independent global subsystems: clear the input force-feedback default, install CrateHaloD3dDrv
 * as the D3D driver factory, heap-allocate and construct the HALO_SOUND_SYSTEM as the active
 * custom sound backend (falling back to null on allocation failure), reserve the physical-memory
 * pool, force the UI off the dedicated UI thread, install hcexCallFrame as the per-frame callback
 * extension, and initialize the loading-screen system.
 *
 * All of these are independent one-time subsystem bootstraps with no data dependency on each
 * other or on this (empty) class; each remains a boundary call/global write, not reversed further
 * in this batch. */

#include "../../headers/hcex/HCEX_INIT_STAT.h"

extern void *operator new(size_t size, const char *file, unsigned int line); // boundary — matches the (size,file,line) `operator new` overload used here (operators cannot have C linkage)
extern "C" void  HALO_SOUND_SYSTEM_ctor(HALO_SOUND_SYSTEM *self);                            // boundary — HALO_SOUND_SYSTEM::HALO_SOUND_SYSTEM
extern "C" void physical_memory_allocate(void);                                                  // boundary
extern "C" void  hcex_loading_screen_init();                                                  // boundary
extern "C" d3dDRIVER *CrateHaloD3dDrv();                                                       // boundary
extern "C" void  hcexCallFrame();                                                             // boundary — the real prototype takes no meaningful args; the DB's 8-int signature is the decompiler's generic varargs-call placeholder for a callback-table slot

extern bool  INP_INIT_FORCE_FEEDBACK;                 // boundary global
extern d3dDRIVER *(*D3D_DRV_CREATOR)();                // boundary global — D3D driver factory hook
extern snd::SYSTEM_CUSTOM *snd_SystemCustom;           // boundary global — `snd::SystemCustom`
extern int   ui_new_USE_UI_THREAD;                     // boundary global — `ui_new::USE_UI_THREAD`
extern void (*gsMsgCallFrameExt)();                    // boundary global — per-frame callback extension hook

HCEX_INIT_STAT::HCEX_INIT_STAT()
{
    INP_INIT_FORCE_FEEDBACK = false;
    D3D_DRV_CREATOR = CrateHaloD3dDrv;

    HALO_SOUND_SYSTEM *soundSystem = static_cast<HALO_SOUND_SYSTEM *>(
        operator new(0x178u, "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 0xAA2u));
    if ( soundSystem )
    {
        HALO_SOUND_SYSTEM_ctor(soundSystem);
        snd_SystemCustom = reinterpret_cast<snd::SYSTEM_CUSTOM *>(soundSystem);
    }
    else
    {
        snd_SystemCustom = nullptr;
    }

    physical_memory_allocate();
    ui_new_USE_UI_THREAD = 0;
    gsMsgCallFrameExt = hcexCallFrame;
    hcex_loading_screen_init();
}
