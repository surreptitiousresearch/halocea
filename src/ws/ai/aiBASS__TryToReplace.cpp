// aiBASS::TryToReplace(const exDATA&, float) @ 0x832441A0
// ?TryToReplace@aiBASS@@MAA_NABVexDATA@1@M@Z
#include "../../headers/ws/ai/aiBASS.h"

// Scan the stream for the entry that best "beats" the incoming exclamation on priority/distance,
// then defer to the 4-arg TryToReplace helper to actually displace it. Until a candidate victim is
// found the comparison baseline is the incoming (exclamation, dist); afterwards it is the current
// best victim's (priority, dist).
bool aiBASS::TryToReplace(const exDATA &exclamation, float dist)
{
    float          bestPriority = -1.0f;
    float          bestDist     = -1.0f;
    exSTREAM_DATA *victim       = nullptr;

    for (ExStreamIter it(exStream.Begin()); !it.IsDone(); ++it)
    {
        exSTREAM_DATA *entry = it.value();
        float refPriority = victim ? bestPriority : exclamation.priority;
        float refDist     = victim ? bestDist     : dist;

        if (IsBetterPriorDist(entry->priority, refPriority, entry->dist, refDist, 0.000001f))
        {
            bestPriority = entry->priority;
            bestDist     = entry->dist;
            victim       = entry;
        }
    }

    if (victim)
        return TryToReplace(victim, exclamation, dist);
    return false;
}
