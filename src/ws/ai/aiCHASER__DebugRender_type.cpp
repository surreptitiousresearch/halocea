// aiCHASER::DebugRender(aiSUSTYPE, aiBRAIN*) @ 0x8320BFA8
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCHASER_boundary.h" // susCache, Spectrum, m3dVZero, gsGetPlayer*
#include "../../headers/ws/ai/nsSUSCACHE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/animINST.h"

// Debug-draw one suspicion type's cached points for `watchee`. The reference position drawn
// depends on the suspicion type: recent-death / expected-player positions come from this chaser,
// player-relative types sample a player's instance position.
void aiCHASER::DebugRender(aiSUSTYPE type, aiBRAIN *watchee)
{
    int playerIdx = 0;
    m3dV refPos[5];
    refPos[0] = m3dVZero;
    unsigned int color = Spectrum[type];
    bool haveExplicitPos = false;

    // Types outside [ASUT_RANDOM..ASUT_MAIN2] (and ASUT_REAL_PLR) fall through to the
    // player-0 sampling default, matching the original range guard.
    if ((unsigned int)type <= (unsigned int)ASUT_MAIN2 && type != ASUT_REAL_PLR)
    {
        switch (type)
        {
        case ASUT_LAST_CORPSE:
            refPos[0] = this->posRecentDeath;
            haveExplicitPos = true;
            break;
        case ASUT_RANDOM:
        case ASUT_LAST_SHOT:
            refPos[0] = this->posExpectedPlayer;
            haveExplicitPos = true;
            break;
        case ASUT_REAL_PLR2:
        case ASUT_LAST_SEEN_PLR2:
        case ASUT_MAIN2:
            playerIdx = 1;
            break;
        case ASUT_MAIN:
        case ASUT_LAST_SEEN_PLR:
        default:
            playerIdx = 0;
            break;
        }
    }

    if (!haveExplicitPos && playerIdx < gsGetPlayerCount())
    {
        entENTITY *player = gsGetPlayer(playerIdx);
        player->pInst->GetPos(refPos);
    }

    short nsID = -1;
    if (watchee && watchee->GetNavSystem())
        nsID = watchee->GetNavSystem()->nsID;

    for (int i = 0; i < susCache.nElem; ++i)
    {
        if (nsID < 0 || nsID == susCache[i].nsID)
            susCache[i].DebugRender(type, color, refPos);
    }
}
