// aiCHASER::DebugRender @ 0x8320C630
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCHASER_boundary.h"
#include "../../headers/ws/ai/nsSUSCACHE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/animINST.h"

void aiCHASER::DebugRender(aiBRAIN *watchee)
{
    if (dbg_isChrLastPlr.value)
    {
        DebugRender(ASUT_LAST_SEEN_PLR, watchee);
        DebugRender(ASUT_LAST_SEEN_PLR2, watchee);
    }
    if (dbg_isChrMain.value)
    {
        DebugRender(ASUT_MAIN, watchee);

        m3dV plrPos = m3dVZero;
        unsigned int color = Spectrum[8];
        if (gsGetPlayerCount() > 1)
        {
            entENTITY *player = gsGetPlayer(1);
            player->pInst->GetPos(&plrPos);
        }

        short nsID = -1;
        if (watchee && watchee->GetNavSystem())
            nsID = watchee->GetNavSystem()->nsID;

        for (int i = 0; i < susCache.nElem; ++i)
        {
            if (nsID < 0 || nsID == susCache[i].nsID)
                susCache[i].DebugRender(ASUT_MAIN2, color, &plrPos);
        }
    }
    if (dbg_isChrLastCorpse.value)
        DebugRender(ASUT_LAST_CORPSE, watchee);
    if (dbg_isChrLastShot.value)
        DebugRender(ASUT_LAST_SHOT, watchee);
    if (dbg_isChrRandom.value)
        DebugRender(ASUT_RANDOM, watchee);
    if (dbg_isChrRealPlr.value)
    {
        DebugRender(ASUT_REAL_PLR, watchee);
        DebugRender(ASUT_REAL_PLR2, watchee);
    }
}
