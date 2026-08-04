#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiCOORDINATOR::MarkWPAsUnused @ 0x832379A0
// ?MarkWPAsUnused@aiCOORDINATOR@@UAAXPAVaiBRAIN@@@Z
//
// Release the waypoint the given brain had claimed: drop it from the used-WP result map and notify
// every group that the waypoint is free again. (The decompiler invented a long trailing argument
// list; the disassembly confirms the sole parameter is `pBrain`.)
void aiCOORDINATOR::MarkWPAsUnused(aiBRAIN *pBrain)
{
    short wpid = GetResultWP(pBrain);
    if (wpid >= 0)
    {
        resultsUsed.Erase(&pBrain);
        int nElem = groups.nElem;
        for (int i = 0; i < nElem; ++i)
            groups[i]->NoticeUnusedWP(wpid);
    }
}
