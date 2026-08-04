#include "../../headers/ws/ai/aiPLATT.h"
#include "../../headers/ws/ai/aiBRAIN.h"      // aiBRAIN complete (WEAK_PTR_BASE base for GetHandle)
#include "../../headers/ws/ds/dsCMP.h"

// aiPLATT::AddAttacker @ 0x83238798
// ?AddAttacker@aiPLATT@@QAAXPBVaiBRAIN@@@Z
//
// Register `brain` as attacking the local player, if not already present. The weak-ptr key adopts
// (and refcounts) the brain's shared handle; PushBack copies it, and the local key releases its
// reference on scope exit.
void aiPLATT::AddAttacker(const aiBRAIN *brain)
{
    if (!brain)
        return;

    ds::WEAK_PTR<aiBRAIN> key(const_cast<aiBRAIN *>(brain));
    dsCMP cmp = {};
    if (attackers.Find<dsCMP, ds::WEAK_PTR<aiBRAIN> >(key, cmp, 0) == -1)
        attackers.PushBack(key);
}
