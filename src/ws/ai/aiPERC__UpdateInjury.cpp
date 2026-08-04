#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_perc_boundary.h"
#include "../../headers/ws/ds/MAP.h"
#include "../../headers/ws/ds/WEAK_PTR.h"
#include <math.h>

// aiPERC::UpdateInjury() @ 0x8329F300
// ?UpdateInjury@aiPERC@@IAAXXZ
//
// Scan the tracked wounds for the first hit from a still-alive shooter whose accumulated damage
// reaches the descriptor's heavy-damage threshold. When found, flush the wound table and raise a
// NoticeHeavyDamaged for that shooter. Disabled when the threshold is the 2000000 sentinel or the
// reaction lock-out timer is still running.
void aiPERC::UpdateInjury()
{
    const dscDESC *desc = pBrain->spDesc.pointee;
    if (!desc)
        return;

    const float heavyThreshold = aiBRAIN_DESC_HeavyDamageThreshold(desc);
    if (fabsf(2000000.0f - heavyThreshold) < 0.000001f) // sentinel: heavy-damage reaction disabled
        return;
    if (injury.timerDisable > 0.000001f)
        return;

    typedef ds::impl::MAP_ITERATOR<const void *, INJURY::WOUND, ds::HASH, ds::CMP,
                                   dsNODE_CACHE_ALLOCATOR> WoundIter;

    entENTITY *heavyShooter = nullptr;
    for (WoundIter it(injury.wounds.Begin()); !it.IsDone(); ++it)
    {
        const INJURY::WOUND *wound = it.value();
        entENTITY *shooter = wound->shooter.pHandle ? wound->shooter.pHandle->pPtr : nullptr;
        if (shooter && wound->amount >= heavyThreshold)
        {
            heavyShooter = shooter;
            break;
        }
    }

    if (heavyShooter)
    {
        injury.wounds.Clear();
        NoticeHeavyDamaged(heavyShooter);
    }
}
