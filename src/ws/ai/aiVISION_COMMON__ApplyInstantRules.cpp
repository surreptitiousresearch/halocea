#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"

// aiVISION_COMMON::ApplyInstantRules @ 0x83292340
// ?ApplyInstantRules@aiVISION_COMMON@@IAA_NXZ
//
// Short-circuit the per-frame vision test for degenerate enemy states. Force zero visibility when
// there is no real enemy or the enemy is estimated-present-but-not-visible; force full visibility
// for units without "real" vision. Returns true when a rule fired (test chain skipped).
bool aiVISION_COMMON::ApplyInstantRules()
{
    aiENEMY_BASE *enemy = pBrain->mind.enemy.pointee;

    float result;
    // est bit 0x200000 = enemy estimated present, bit 0x80000 = enemy currently visible.
    if (enemy->IsZero()
        || enemy->Is(nullptr)
        || ((pBrain->st.pointee->est.val & 0x200000) && !(pBrain->st.pointee->est.val & 0x80000)))
    {
        result = 0.0f;
    }
    else
    {
        aiBRAIN_DESC *desc = reinterpret_cast<aiBRAIN_DESC *>(pBrain->spDesc.pointee);
        if (desc->abil.visionReal)
            return false; // real vision arms run the full test instead of an instant result
        result = 100.0f;
    }

    StoreLatestTest(result);
    viewPercent = result;
    return true;
}
