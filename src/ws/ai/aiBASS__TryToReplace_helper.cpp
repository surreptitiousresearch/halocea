#include "../../headers/ws/ai/aiBASS.h"

// aiBASS::TryToReplace (4-arg helper) @ 0x83240810
// ?TryToReplace@aiBASS@@MAA_NPBVexSTREAM_DATA@1@ABVexDATA@1@M@Z
//
// If `victim` exists and the incoming `exclamation` at `dist` wins the priority/distance comparison
// against it, evict `victim` from the stream and report success. Called by the 2-arg
// TryToReplace(const exDATA&, float) selector once it has chosen the best victim.
bool aiBASS::TryToReplace(const exSTREAM_DATA *victim, const exDATA &exclamation, float dist)
{
    if (!victim ||
        !IsBetterPriorDist(victim->priority, exclamation.priority, victim->dist, dist, distanceDelta))
        return false;

    RemoveExFromStream(victim);
    return true;
}
