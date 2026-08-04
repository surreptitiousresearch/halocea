#include "../../headers/ws/ai/aiISA.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"  // dsSPrintf
#include "../../headers/ws/ent/entSND_NOTICE.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiISA::NotifySND() @ 0x832A0910
// ?NotifySND@aiISA@@UAAXABVentSND_NOTICE@@@Z
//
// Virtual: a sound was heard. Ignored unless the HEAR report is enabled. The hearing radius is
// selected by the sound's loudness bucket; if the sound is within it (squared distance), raise a
// HEAR alert whose reason names the emitter and sound.
void aiISA::NotifySND(const entSND_NOTICE &notice)
{
    if ((state.val & AI_ISAST_REPORT_HEAR) == 0)
        return;

    m3dV pos = notice.pos;
    const float dist2 = pBrain->Dist_2(&pos);
    const aiSTATUS *st = pBrain->st.pointee;

    float radHear = 0.0f;
    switch (notice.volume)
    {
    case SND_AI_VAL_LOW:  radHear = st->perc.senses.radHearWalk; break;
    case SND_AI_VAL_MED:  radHear = st->perc.senses.radHearRun;  break;
    case SND_AI_VAL_HIGH: radHear = st->perc.senses.radHearShot; break;
    default:              break; // SND_AI_VAL_NA / SND_AI_VAL_LAST — no reaction
    }

    if (dist2 < radHear * radHear)
    {
        aiISA::ALERT_PARAMS params = {
            AI_ALERT_HEAR,
            pos,
            dsSPrintf("HEAR(%s[%s] - '%s')", notice.nameEnt.CStr(), notice.nameEntDbg.CStr(),
                      notice.nameSnd.CStr()),
            true,   // isAlertTeam
            false,  // isUseDelay
        };
        Alert(params);
    }
}
