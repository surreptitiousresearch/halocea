#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h"

// aiEXIFACE::RegisterWeaponUser @ 0x82D31AB0
// ?RegisterWeaponUser@aiEXIFACE@@QAAXPAVpropWPN_AI_USABLE@@PAVentENTITY@@@Z
//
// `user` is really a derived entity whose weapon-manager block lives PAST the entENTITY base
// (offset 0x1C0 > sizeof(entENTITY)=0x1BC). Those offsets are that subclass's undocumented
// internal wiring (boundary layout — reproduced as raw pointer arithmetic, not fabricated):
//   manager = *(void**)(user + 0x1C0)
//   handler = *(void**)(manager + 0x10)                 [weapon-user list head]
//   handler->vtbl[0x44/4](handler, usable)              virtual "add user"
void aiEXIFACE::RegisterWeaponUser(propWPN_AI_USABLE *usable, entENTITY *user)
{
    void *manager = *reinterpret_cast<void **>(reinterpret_cast<char *>(user) + 0x1C0);
    void *handler = *reinterpret_cast<void **>(reinterpret_cast<char *>(manager) + 0x10);

    if (!IGNORE_STRONG_ASSERT && !handler)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "wpn",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai11\\a11_stub.cpp", 576,
            STRONG_ASSERT_INFO_STUB);

    typedef void (*AddUserFn)(void *, propWPN_AI_USABLE *);
    void **vtbl = *reinterpret_cast<void ***>(handler);
    reinterpret_cast<AddUserFn>(vtbl[0x44 / sizeof(void *)])(handler, usable);
}
