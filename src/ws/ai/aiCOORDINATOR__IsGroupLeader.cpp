#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// aiCOORDINATOR::IsGroupLeader @ 0x8322EBE8
// ?IsGroupLeader@aiCOORDINATOR@@UAA_NPAVaiBRAIN@@@Z
//
// True when `pBrain` leads its coordinated group. False when it has no group.
bool aiCOORDINATOR::IsGroupLeader(aiBRAIN *pBrain)
{
    aiCOORD_GROUP *group = GetGroup_2(pBrain);
    if (!group)
        return false;
    // group->IsGroupLeader takes the actor interface; the brain is passed through as its actor.
    return group->IsGroupLeader(reinterpret_cast<iaIACTOR *>(pBrain));
}
