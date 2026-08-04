#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"

// aiCOORDINATOR::NoticeRemoveOtherGroup @ 0x83235620
// ?NoticeRemoveOtherGroup@aiCOORDINATOR@@IBAXPAVaiCOORD_GROUP@@@Z
//
// Notify every group except `grp` that `grp` is being removed.
void aiCOORDINATOR::NoticeRemoveOtherGroup(aiCOORD_GROUP *grp) const
{
    int nElem = groups.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        if (groups[i] != grp)
            groups[i]->NoticeRemoveOtherGroup(grp);
    }
}
