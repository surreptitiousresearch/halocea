#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/ws/os/os_boundary.h"

// HALO_SOUND_SYSTEM::~HALO_SOUND_SYSTEM() @ 0x836C0B90
// Resets the vtable back to the snd::SYSTEM_CUSTOM base (standard destructor vptr unwind),
// then tears down every owned container. tagInfoPool and customSounds hold trivially-
// destructible elements (raw TAG_INFO*/HALO_CUSTOM_SOUND with no owned sub-resources of their
// own beyond FMOD handles the pool/vector itself doesn't own), so those two are released with a
// bare dlFree of the backing store rather than a full vector destructor call -- reproduced
// verbatim from disasm, matching the pattern already established elsewhere in this codebase
// for `dsVECTOR<T,8>` element types with no per-element teardown.
HALO_SOUND_SYSTEM::~HALO_SOUND_SYSTEM()
{
    __vftable = (snd::SYSTEM_CUSTOM_vtbl *)&snd::SYSTEM_CUSTOM::vftable;

    tagInfoPool.policy.Free();
    dlFree(tagInfoPool.policy.freeNodes.pData);
    dlFree(tagInfoPool.usedNodes.pData);

    playToEnd.~dsVECTOR<HALO_CHANNEL, 8>();
    paramsWildcard.~dsVECTOR<HALO_SOUND_PARAMS, 8>();
    params.~dsVECTOR<HALO_SOUND_PARAMS, 8>();
    dlFree(customSounds.pData);
    channels.~dsVECTOR<HALO_CHANNEL, 8>();
    preloadList.~dsVECTOR<HALO_PERM_SOUND, 8>();
    soundLists.~dsVECTOR<HALO_SOUND_LIST, 8>();

    dbgLock.Term();

    // snd::SYSTEM_CUSTOM base subobject destruction happens implicitly at end of scope.
}
