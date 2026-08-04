#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"

// aiCOORDINATOR::GetGroupMembersCount @ 0x8322EC60
// ?GetGroupMembersCount@aiCOORDINATOR@@UAAHPAVaiBRAIN@@@Z
//
// Number of members in `pBrain`'s coordinated group, or 0 when it has no group.
int aiCOORDINATOR::GetGroupMembersCount(aiBRAIN *pBrain)
{
    aiCOORD_GROUP *group = GetGroup_2(pBrain);
    if (!group)
        return 0;
    return group->GetMembersCount();
}
