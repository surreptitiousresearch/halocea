#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h" // aiWATCHER::CanAcceptEnemy
#include "../../headers/ws/dbg/STRONG_ASSERT2_HELPER.h"

extern int IGNORE_STRONG_ASSERT; // ai08 strong-assert bypass. boundary.

// aiFILT_ENEMY::PassEnemiesCount(aiBRAIN*) @ 0x83220718
// ?PassEnemiesCount@aiFILT_ENEMY@@QAAXPAVaiBRAIN@@@Z
// Drop every FEE whose watched unit will not accept `pBrain` as an enemy.
void aiFILT_ENEMY::PassEnemiesCount(aiBRAIN *pBrain)
{
    for (int i = arr.nElem - 1; i >= 0; --i)
    {
        if (!IGNORE_STRONG_ASSERT && i >= arr.nElem)
            STRONG_ASSERT2_HELPER::asserd<int, int>(
                "idx >= 0 && idx < nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 567,
                "idx", i, "nElem", arr.nElem);

        if (!arr.pData[i].hold->watcher->CanAcceptEnemy(pBrain))
            arr.Erase(i, 1);
    }
}
