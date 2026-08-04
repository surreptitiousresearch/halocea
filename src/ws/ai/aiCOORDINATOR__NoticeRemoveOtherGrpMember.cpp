#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiCOORDINATOR::NoticeRemoveOtherGrpMember @ 0x83235420
// ?NoticeRemoveOtherGrpMember@aiCOORDINATOR@@UBAXPAVaiCOORD_GROUP@@PAVaiBRAIN@@@Z
//
// Broadcast "a member of group `grp` (the brain `pBrain`) was removed" to every OTHER registered
// group, so each can drop any cross-group reference it kept to that unit.
void aiCOORDINATOR::NoticeRemoveOtherGrpMember(aiCOORD_GROUP *grp, aiBRAIN *pBrain)
{
    int nElem = this->groups.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        if (this->groups[i] != grp)
            this->groups[i]->NoticeRemoveOtherGrpMember(grp, pBrain);
    }
}
