#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // aiENEMY_BASE

// aiGRASS::HandleSmokingOut @ 0x83183AB8
// ?HandleSmokingOut@aiGRASS@@IAAXPBVaiBRAIN@@@Z  (protected)
//
// When the brain's currently tracked enemy peeker is one of our smoke-out victims, destroy that
// watcher and drop it from the victims list.
void aiGRASS::HandleSmokingOut(const aiBRAIN *shooter)
{
    if (!shooter)
        return;

    aiENEMY_BASE *enemy = shooter->mind.enemy.pointee;
    if (!enemy || !enemy->Is(nullptr))
        return;

    aiWATCHER *peeker = enemy->GetPeeker();
    if (!peeker)
        return;

    bool isVictim = false;
    for (ds::LIST<aiWATCHER *>::DATA *node = this->victims.head; node; node = node->next)
    {
        if (node->val == peeker)
        {
            isVictim = true;
            break;
        }
    }

    if (isVictim)
    {
        aiWATCHER::Destroy(peeker);
        this->victims.Erase(peeker);
    }
}
