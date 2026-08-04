#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiBRAIN.h"     // aiBRAIN::mind
#include "../../headers/ws/ai/aiMIND.h"      // aiMIND::IsAcceptableEnemy
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/dbg/STRONG_ASSERT2_HELPER.h"

extern int IGNORE_STRONG_ASSERT; // ai08 strong-assert bypass. boundary.

// aiFILT_ENEMY::PassAllyOpp(aiBRAIN*) @ 0x83220488
// ?PassAllyOpp@aiFILT_ENEMY@@QAAXPAVaiBRAIN@@@Z
// Drop every FEE whose watched unit `viewer`'s mind does not accept as an enemy (removes allies).
void aiFILT_ENEMY::PassAllyOpp(aiBRAIN *viewer)
{
    aiMIND *mind = &viewer->mind;
    for (int i = arr.nElem - 1; i >= 0; --i)
    {
        if (!IGNORE_STRONG_ASSERT && i >= arr.nElem)
            STRONG_ASSERT2_HELPER::asserd<int, int>(
                "idx >= 0 && idx < nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 567,
                "idx", i, "nElem", arr.nElem);

        if (!mind->IsAcceptableEnemy(arr.pData[i].hold->watcher))
            arr.Erase(i, 1);
    }
}
