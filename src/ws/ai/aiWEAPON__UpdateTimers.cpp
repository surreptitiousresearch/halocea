#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiWEAPON::UpdateTimers @ 0x832939A0
// ?UpdateTimers@aiWEAPON@@MAAXM@Z
//
// Per-frame aging of the weapon module's timers: the trajectory checker, every strike-vars block
// (flagged active only for the currently-selected strike), the kill-hang timer, and the delayed
// OSS (on-strike-start) window — firing OnStrikeStart when that window closes for an armed strike.
void aiWEAPON::UpdateTimers(float dt)
{
    trc.UpdateTimers(dt);

    for (int i = 0; i < svarsList.nEntry; ++i) {
        aiS_VARS &v = svarsList[i];
        v.UpdateTimers(dt, &v == tieSelected.vars);
    }

    m3dUpdateTimeField(&timerHackKillHang, dt);

    if (((unsigned int)pBrain->st.pointee->wpn.val >> 2) & 1
        && m3dUpdateTimeField(&timerDelayedOSS, dt)) {
        pBrain->st.pointee->wpn.val &= ~4;
        aiBRAIN *brain = pBrain;
        if (((unsigned int)brain->st.pointee->wpn.val >> 1) & 1) {
            dsSTRID name;
            name.id = tieSelected.name.id;
            brain->OnStrikeStart(name);
        }
    }
}
