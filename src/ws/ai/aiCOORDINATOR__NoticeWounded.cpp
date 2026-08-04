#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"

// aiCOORDINATOR::NoticeWounded @ 0x8322EB48
// ?NoticeWounded@aiCOORDINATOR@@UAAXPAVaiBRAIN@@M@Z
//
// Forward a wound event for `pBrain` to that brain's coordinated group, if it has one.
void aiCOORDINATOR::NoticeWounded(aiBRAIN *pBrain, float amount)
{
    aiCOORD_GROUP *group = GetGroup_2(pBrain);
    if (group)
        group->NoticeWounded(pBrain, amount);
}
