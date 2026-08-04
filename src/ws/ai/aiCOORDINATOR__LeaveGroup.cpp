#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"

// aiCOORDINATOR::LeaveGroup @ 0x83236928
// ?LeaveGroup@aiCOORDINATOR@@UAAXPAVaiBRAIN@@@Z
//
// Remove `pBrain` from whatever group it is in, then prune any now-empty/invalid groups.
void aiCOORDINATOR::LeaveGroup(aiBRAIN *pBrain)
{
    aiCOORD_GROUP *grp = GetGroup_2(pBrain);
    if (grp)
    {
        // vtable+84: aiCOORD_GROUP::RemoveMember(aiBRAIN*) — the exact slot is not individually
        // resolved this batch, so dispatch it through the raw vtable offset the caller used.
        (*reinterpret_cast<void (**)(aiCOORD_GROUP *, aiBRAIN *)>(
            *reinterpret_cast<char **>(grp) + 84))(grp, pBrain);
        RemoveDeletedGroups();
    }
}
