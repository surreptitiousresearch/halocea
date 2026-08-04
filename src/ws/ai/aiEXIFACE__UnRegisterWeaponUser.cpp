#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h"

// aiEXIFACE::UnRegisterWeaponUser @ 0x82D31B40
// ?UnRegisterWeaponUser@aiEXIFACE@@QAAXPAVpropWPN_AI_USABLE@@PAVentENTITY@@@Z
//
// Mirror of RegisterWeaponUser (see that file for the boundary offset rationale); dispatches
// the "remove user" virtual at vtable slot 0x48 instead of 0x44.
void aiEXIFACE::UnRegisterWeaponUser(propWPN_AI_USABLE *usable, entENTITY *user)
{
    void *manager = *reinterpret_cast<void **>(reinterpret_cast<char *>(user) + 0x1C0);
    void *handler = *reinterpret_cast<void **>(reinterpret_cast<char *>(manager) + 0x10);

    if (!IGNORE_STRONG_ASSERT && !handler)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "wpn",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai11\\a11_stub.cpp", 588,
            STRONG_ASSERT_INFO_STUB);

    typedef void (*RemoveUserFn)(void *, propWPN_AI_USABLE *);
    void **vtbl = *reinterpret_cast<void ***>(handler);
    reinterpret_cast<RemoveUserFn>(vtbl[0x48 / sizeof(void *)])(handler, usable);
}
