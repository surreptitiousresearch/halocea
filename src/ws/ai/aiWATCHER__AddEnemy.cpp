#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// aiWATCHER::AddEnemy @ 0x8328E298
// ?AddEnemy@aiWATCHER@@UAAXPBVaiBRAIN@@@Z
// Virtual: register a live `enemy` against this watcher, bumping either the melee or the ranged
// attacker count depending on the enemy's AI_MIST_MELEE_UNIT status flag.
void aiWATCHER::AddEnemy(const aiBRAIN *enemy)
{
    if (!enemy || enemy->IsDead())
        return;

    if (enemy->st.pointee->mind.val & 0x10000) // AI_MIST_MELEE_UNIT
        ++countMeleeEnemies;
    else
        ++countRangedEnemies;
}
