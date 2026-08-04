#include "../../headers/ws/ai/aiPLATT.h"
#include "../../headers/ws/ai/aiBRAIN.h"      // aiBRAIN complete (WEAK_PTR_BASE base for GetHandle)
#include "../../headers/ws/ds/dsCMP.h"

// aiPLATT::RemoveAttacker @ 0x83238628
// ?RemoveAttacker@aiPLATT@@QAAXPBVaiBRAIN@@@Z
//
// Drop `brain` from the player's attacker set if present.
void aiPLATT::RemoveAttacker(const aiBRAIN *brain)
{
    if (!brain)
        return;

    ds::WEAK_PTR<aiBRAIN> key(const_cast<aiBRAIN *>(brain));
    dsCMP cmp = {};
    int idx = attackers.Find<dsCMP, ds::WEAK_PTR<aiBRAIN> >(key, cmp, 0);
    if (idx != -1)
        attackers.Erase(idx, 1);
}
