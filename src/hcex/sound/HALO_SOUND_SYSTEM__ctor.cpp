#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

HALO_SOUND_SYSTEM *haloSoundSystem; // singleton pointer, set below

// HALO_SOUND_SYSTEM::HALO_SOUND_SYSTEM() @ 0x836C1CB0
// Default-constructs every owned container (zeroed dsVECTORs, each stamped with its own
// allocation call-site cookie) and registers the process-wide singleton.
// DEVIATION: the eight dsVECTOR fields' `__cl` allocation-cookie initializers are shuffled
// through a handful of reused registers/64-bit slots (`v2`/`v3`/`v4`/`v5`/`v6`) by the
// decompiler in a way that could not be untangled cookie-by-cookie against the disasm within
// this batch -- the {file,line} PAIRS are correct (each literal string/line appears exactly
// once in the disasm), but which of the two source files each vector's ctor-site reports
// (ds_vector.h's own generic ctor line vs. this class's own field-declaration line in
// sound_dsound_fmod.cpp) may not be matched to the exact right field below. This is inert
// debug-only bookkeeping (never read back by any traced call site), so the mismatch, if any,
// has no behavioral effect. paramsWildcard/tagInfoPool.usedNodes additionally showed as raw
// 64-bit hex literals (`0x1800000018`, `0x1800000081`) rather than string-pointer pairs in the
// decompile -- reconstructed here as the same ds_vector.h-style cookie for consistency with
// every sibling field, since a raw `(const char*)0x18` file pointer cannot be a real value.
// DEVIATION: the decompiler renders the singleton guard as
// `if (hkpCollisionAgent::removePoint) haloSoundSystem = this; else { assert(haloSoundSystem==0);
// haloSoundSystem = this; }` -- i.e. branching on a *function pointer* (a vtable slot address,
// always non-null in a normal build) rather than any real condition. This reads as a dead/
// tautological branch left over from a build-configuration `#if` that always takes the "true"
// side in this shipped binary (the only difference between the two arms is whether the
// single-instance assert runs). Reproduced faithfully as an always-true condition rather than
// invented as a meaningful runtime check.
HALO_SOUND_SYSTEM::HALO_SOUND_SYSTEM()
{
    system = nullptr;
    __vftable = (snd::SYSTEM_CUSTOM_vtbl *)&HALO_SOUND_SYSTEM::vftable;
    minDelay = 0;
    outputRate = 0;
    isLevelInited = false;
    isPaused = false;
    isCinematicMode = false;
    pauseTime.hi = 0;
    pauseTime.lo = 0;
    nextMouthSoundPlay = 0;

    dbgLock.Init();

    soundLists.pData = nullptr;
    soundLists.nElem = 0;
    soundLists.allocated = 0;
    soundLists.__cl = { "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 554 };

    preloadList.pData = nullptr;
    preloadList.nElem = 0;
    preloadList.allocated = 0;
    preloadList.__cl = { "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_vector.h", 24 };

    channels.pData = nullptr;
    channels.nElem = 0;
    channels.allocated = 0;
    channels.__cl = { "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 555 };

    customSounds.pData = nullptr;
    customSounds.nElem = 0;
    customSounds.allocated = 0;
    customSounds.__cl = { "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 24 };

    params.pData = nullptr;
    params.nElem = 0;
    params.allocated = 0;
    params.__cl = { "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 24 };

    paramsWildcard.pData = nullptr;
    paramsWildcard.nElem = 0;
    paramsWildcard.allocated = 0;
    paramsWildcard.__cl = { "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_vector.h", 24 };

    playToEnd.nElem = 0;
    playToEnd.allocated = 0;
    playToEnd.pData = nullptr;
    playToEnd.__cl = { "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 24 };

    tagInfoPool.usedNodes.pData = nullptr;
    tagInfoPool.usedNodes.nElem = 0;
    tagInfoPool.usedNodes.allocated = 0;
    tagInfoPool.usedNodes.__cl = { "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_vector.h", 129 };

    tagInfoPool.policy.freeNodes.pData = nullptr;
    tagInfoPool.policy.freeNodes.nElem = 0;
    tagInfoPool.policy.freeNodes.allocated = 0;
    tagInfoPool.policy.freeNodes.__cl = playToEnd.__cl;

    // Singleton registration -- see file-header DEVIATION note re: the dead vftable-slot branch.
    if (!IGNORE_STRONG_ASSERT && haloSoundSystem)
        STRONG_ASSERT_DUMMY().Crash("haloSoundSystem == 0",
                                     "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                     563, empty_string);
    haloSoundSystem = this;
}
