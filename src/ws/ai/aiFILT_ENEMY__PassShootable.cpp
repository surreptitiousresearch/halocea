#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/dbg/STRONG_ASSERT2_HELPER.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;              // ai08 strong-assert bypass. boundary.
extern unsigned char STRONG_ASSERT_INFO_STUB; // byte_8200155A. boundary.

// entENTITY.stateEnt bit 10 (0x400): entity is shootable.
static const unsigned int ENT_SHOOTABLE_BIT = 1u << 10;

// aiFILT_ENEMY::PassShootable(aiBRAIN*) @ 0x83220560
// ?PassShootable@aiFILT_ENEMY@@QAAXPAVaiBRAIN@@@Z
// If any FEE's entity is shootable, drop every non-shootable FEE; otherwise leave the set alone.
void aiFILT_ENEMY::PassShootable(aiBRAIN * /*viewer*/)
{
    int nElem = arr.nElem;
    bool anyShootable = false;

    for (int i = 0; i < nElem; ++i)
    {
        aiFEE &fee = arr[i];
        entENTITY *ent = fee.hold->watcher->GetEntity();
        if (!IGNORE_STRONG_ASSERT && !ent)
            STRONG_ASSERT_DUMMY::Crash(nullptr, "entCand",
                "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_fe.cpp", 335,
                STRONG_ASSERT_INFO_STUB);

        if ((ent->stateEnt.val & ENT_SHOOTABLE_BIT) != 0)
        {
            anyShootable = true;
            break;
        }
    }

    if (!anyShootable)
        return;

    for (int i = arr.nElem - 1; i >= 0; --i)
    {
        if (!IGNORE_STRONG_ASSERT && i >= arr.nElem)
            STRONG_ASSERT2_HELPER::asserd<int, int>(
                "idx >= 0 && idx < nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 567,
                "idx", i, "nElem", arr.nElem);

        entENTITY *ent = arr.pData[i].hold->watcher->GetEntity();
        if (!IGNORE_STRONG_ASSERT && !ent)
            STRONG_ASSERT_DUMMY::Crash(nullptr, "entCand",
                "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_fe.cpp", 352,
                STRONG_ASSERT_INFO_STUB);

        if ((static_cast<unsigned int>(ent->stateEnt.val) & ENT_SHOOTABLE_BIT) == 0)
            arr.Erase(i, 1);
    }
}
