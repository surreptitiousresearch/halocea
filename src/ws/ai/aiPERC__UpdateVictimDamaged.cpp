#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/dsc/dscDESC.h"

// aiPERC::UpdateVictimDamaged @ 0x8329D4F8
// ?UpdateVictimDamaged@aiPERC@@IAAXXZ
//
// Once the accumulation lock-out has expired and either a kill was scored or the accumulated damage
// crossed the descriptor threshold (cell 50), report it to the brain (unless still firing) and
// reset the accumulator.
void aiPERC::UpdateVictimDamaged()
{
    if (infliction.timerLocked <= 0.000001f
        && (infliction.killed
            || infliction.dmgAccumulated >= *(float *)&pBrain->spDesc.pointee[50].pBrand)) {
        aiWEAPON_BASE *wpn = pBrain->body.pointee->wpn.pointee;
        if (!wpn->IsShootingNow()) {
            pBrain->OnEnemyDamaged(infliction.dmgAccumulated, infliction.killed);
            infliction.killed = 0;
            infliction.dmgAccumulated = 0.0f;
            infliction.timerLocked = 0.0f;
        }
    }
}
