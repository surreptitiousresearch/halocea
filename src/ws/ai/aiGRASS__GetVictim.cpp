#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // full aiENEMY_BASE (GetPeeker)

// --- boundary type (full layout is the next frontier) ---
// A coordinated squad/group; only the member/target accessors used here are declared.
struct aiCOORD_GROUP {
    int              GetMembersCount() const;   // vtbl+0x60
    bool             IsTargetValid() const;     // vtbl+0x84
    const aiBRAIN   *GetBrain(int idx) const;   // vtbl+0x44
};

// aiGRASS::GetVictim @ 0x83182E98
// Choose the "victim" watcher of a coordinated group. When the group has a valid target, return
// that first member's enemy peeker; otherwise scan members and return the enemy peeker with the
// greatest hideout time (timeInHO).
aiWATCHER *aiGRASS::GetVictim(aiCOORD_GROUP *group) const
{
    if (group->GetMembersCount() <= 0)
        return nullptr;

    if (group->IsTargetValid())
    {
        const aiBRAIN *brain = group->GetBrain(0);
        if (!brain)
            return nullptr;
        aiENEMY_BASE *enemy = brain->mind.enemy.pointee;
        if (!enemy)
            return nullptr;
        return enemy->GetPeeker();
    }

    int   bestIdx = -1;
    float bestScore = 0.0f;
    int   count = group->GetMembersCount();
    if (count <= 0)
        return nullptr;

    for (int i = 0; i < count; ++i)
    {
        const aiBRAIN *brain = group->GetBrain(i);
        if (!brain)
            continue;
        aiENEMY_BASE *enemy = brain->mind.enemy.pointee;
        if (!enemy)
            continue;
        aiWATCHER *peeker = enemy->GetPeeker();
        if (peeker && peeker->timeInHO > bestScore)
        {
            bestScore = peeker->timeInHO;
            bestIdx = i;
        }
    }

    if (bestIdx <= -1)
        return nullptr;

    const aiBRAIN *best = group->GetBrain(bestIdx);
    return best->mind.enemy.pointee->GetPeeker();
}
