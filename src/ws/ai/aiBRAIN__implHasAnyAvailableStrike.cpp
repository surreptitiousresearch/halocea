// aiBRAIN::implHasAnyAvailableStrike @ 0x832B2CD8
// ?implHasAnyAvailableStrike@aiBRAIN@@UAA_NXZ  (virtual)
//
// True when the brain's best-behaviour module has a top behaviour and that behaviour currently offers
// at least one available strike.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/bhvBEHAV.h"

bool aiBRAIN::implHasAnyAvailableStrike()
{
    aiBEST_BASE *best = this->mind.best.pointee;
    if (!best)
        return false;

    bhvBEHAV *top = best->GetTopBehavior();
    return top && top->HasAnyAvailableStrike();
}
