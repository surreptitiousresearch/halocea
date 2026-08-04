#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// aiWATCHER::RemoveEnemy @ 0x8328E320
// ?RemoveEnemy@aiWATCHER@@UAAXPBVaiBRAIN@@@Z
// Virtual: deregister `enemy`, decrementing the melee or ranged attacker count (chosen by the
// enemy's AI_MIST_MELEE_UNIT status flag). Counts are clamped at zero.
void aiWATCHER::RemoveEnemy(const aiBRAIN *enemy)
{
    if (!enemy)
        return;

    if (enemy->st.pointee->mind.val & 0x10000) // AI_MIST_MELEE_UNIT
    {
        if (countMeleeEnemies > 0)
            --countMeleeEnemies;
    }
    else
    {
        if (countRangedEnemies > 0)
            --countRangedEnemies;
    }
}
