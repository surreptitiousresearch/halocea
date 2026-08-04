#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// aiWATCHER::CanAcceptEnemy @ 0x8328E370
// ?CanAcceptEnemy@aiWATCHER@@UBA_NPBVaiBRAIN@@@Z
// Virtual const: true when this watcher still has capacity (current count <= cap) for another
// attacker of `enemy`'s kind (melee vs ranged, chosen by AI_MIST_MELEE_UNIT).
bool aiWATCHER::CanAcceptEnemy(const aiBRAIN *enemy) const
{
    if (!enemy)
        return false;

    // The current-count and cap accessors are non-const virtuals on the base.
    aiWATCHER *self = const_cast<aiWATCHER *>(this);

    int cap;
    int current;
    if (enemy->st.pointee->mind.val & 0x10000) // AI_MIST_MELEE_UNIT
    {
        cap = self->GetMaxMeleeEnemies();
        current = self->GetMeleeEnemies();
    }
    else
    {
        cap = self->GetMaxRangedEnemies();
        current = self->GetRangedEnemies();
    }
    return cap >= current;
}
