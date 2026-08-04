#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"

// aiFILT_ENEMY::GetClosest @ 0x83220C10
// ?GetClosest@aiFILT_ENEMY@@QAAPAVentENTITY@@XZ
//
// The entity of the candidate with the smallest dist4cmp weight (null when empty).
entENTITY *aiFILT_ENEMY::GetClosest()
{
    const int nElem = arr.nElem;
    if (nElem <= 0)
        return nullptr;

    int   bestIdx  = -1;
    float bestDist = 888888.88f; // decompiler-observed sentinel

    for (int i = 0; i < nElem; ++i)
    {
        const aiFEE &fee = arr.pData[i];
        if (fee.dist4cmp < bestDist)
        {
            bestDist = fee.dist4cmp;
            bestIdx  = i;
        }
    }

    if (bestIdx == -1)
        return nullptr;

    aiWTC_HOLDER *hold = arr.pData[bestIdx].hold;
    return hold->watcher->GetEntity();
}
