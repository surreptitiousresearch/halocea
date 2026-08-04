// aiBRAIN::implForceWar @ 0x83286D68
// ?implForceWar@aiBRAIN@@UAAXXZ  (virtual)
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiMIND.h"

// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

// SSL op: force this AI into the "war" battle state. Rejected (logged) if invoked re-entrantly from
// within an OnBattleChanged() event. Otherwise sets the force-war gen flag, clears force-peace, sets
// the perceived-war mind flag, and fires OnBattleChanged when transitioning from peace to war.
void aiBRAIN::implForceWar()
{
    aiSTATUS *status = st.pointee;
    // gen bit 0x2000000 = AI_GEST_OBC_EVENT_IN_PROCESS (inside an OnBattleChanged event)
    if (status->gen.val & 0x2000000)
    {
        _apLog("~AI,Ssl,Error~'%s' : ForceWar() called within OnBattleChanged(). Ignoring.",
               names.nameInst);
        return;
    }

    // mind bit 0x80 = AI_MIST_PERC_WAR (perceived war)
    bool wasPeace = (status->mind.val & 0x80) == 0;
    status->gen.val |= 0x1;          // AI_GEST_BATTLE_FORCE_WAR
    status->gen.val &= ~0x2;         // clear AI_GEST_BATTLE_FORCE_PEACE
    status->mind.val |= 0x80;        // AI_MIST_PERC_WAR
    if (wasPeace)
        mind.CallSSL_OnBattleChanged();
}
