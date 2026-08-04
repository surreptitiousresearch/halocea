// aiCHASER::ReportHeardPlayer @ 0x8320C2A8
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/nsSUSCACHE.h"
#include "../../headers/ws/nav/navSYS.h"

// Record a heard-player position for nav-system `sys` into its suspicion cache, once per frame.
void aiCHASER::ReportHeardPlayer(const m3dV *pos, navSYS *sys)
{
    this->posExpectedPlayer = *pos;

    nsSUSCACHE *cache = FindCache(sys);
    if (cache && cache->isListenCurFrame)
    {
        cache->AssocUpdate(ASUT_LAST_SHOT, this->posExpectedPlayer);
        cache->isListenCurFrame = 0;
    }
}
