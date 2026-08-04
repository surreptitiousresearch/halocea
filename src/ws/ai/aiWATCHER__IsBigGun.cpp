#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"
#include "../../headers/ws/ctrl/ctrlPROP_ID.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiWATCHER::IsBigGun @ 0x8328DAE8
// ?IsBigGun@aiWATCHER@@UAA_NXZ
// Virtual: true when the watcher's current weapon is a rocket-launcher variant. The set of "big gun"
// type ids is built once (lazily) into a function-local static and compared by interned id.
bool aiWATCHER::IsBigGun()
{
    wpnLIST_BASE *weaponList = GetWeaponList();
    if (!weaponList)
        return false;

    wpnWEAPON *weapon = weaponList->GetWeaponCur();
    if (!weapon)
        return false;

    dsSTRID curType = weapon->GetType().GetStr();

    // Guarded one-time init in the binary (guard byte _S1_287); the C++ static equivalent.
    static dsSTRID guns[3] = {
        dsSTRID("ROCKETLAUNCHER", false),
        dsSTRID("ROCKETLAUNCHER_02", false),
        dsSTRID("ROCKETLAUNCHER_03", false),
    };

    for (int i = 0; i < 3; ++i)
    {
        if (guns[i].id == curType.id)
            return true;
    }
    return false;
}
