#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"

// aiCOORDINATOR::GetGroupIdx @ 0x83235540
// ?GetGroupIdx@aiCOORDINATOR@@UBAHPAVaiBRAIN@@@Z
//
// Index of the group `pBrain` belongs to, or -1 if it is not a member of any group.
int aiCOORDINATOR::GetGroupIdx(aiBRAIN *pBrain) const
{
    int nElem = groups.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        // &pBrain->aiBRAIN_IFACE — the aiBRAIN_IFACE base subobject lives at offset 0 of aiBRAIN.
        if (groups[i]->IsContain(reinterpret_cast<iaIACTOR *>(pBrain)))
            return i;
    }
    return -1;
}
