#include "../../headers/ws/ai/aiISA.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// aiISA::ProcessVision() @ 0x832A0FF0
// ?ProcessVision@aiISA@@IAAXXZ
//
// Protected: when SEE reporting is on and the tracked enemy is a real, distinct entity that is
// currently visible (either the manual view-percent threshold or the estimate "visible" bit),
// raise a SEE alert naming the seen entity at its nav position.
void aiISA::ProcessVision()
{
    if ((state.val & AI_ISAST_REPORT_SEE) == 0)
        return;

    aiENEMY_BASE *enemy = pBrain->mind.enemy.pointee;
    if (!enemy)
        return;
    if (enemy->Is(nullptr))
        return;

    const aiSTATUS *st = pBrain->st.pointee;
    bool visible;
    if (st->perc.shouldUseManualViewPercent)
        visible = (float)st->perc.viewPrecent > st->perc.viewPercForAlert;
    else
        visible = ((unsigned int)st->est.val >> 5) & 1;

    if (!visible)
        return;

    dsTSTRING<char> reason;
    reason.UnsafeInit("SEE(", -1, 0);
    aiWATCHER *peeker = pBrain->mind.enemy.pointee->GetPeeker();
    if (peeker)
        reason += peeker->GetEntity()->GetName();
    reason.Insert(reason.pBuffer->strLen, ")", -1);

    aiISA::ALERT_PARAMS params = {
        AI_ALERT_SEE,
        st->enemy.posNav,
        reason,
        true,   // isAlertTeam
        false,  // isUseDelay
    };
    Alert(params);
}
