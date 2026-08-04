#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiSENSES.h"
#include "../../headers/ws/ai/aiSENSES_setsen_boundary.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_perc_boundary.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // _apLog

// aiBRAIN::implSetSenses() @ 0x832A06C8
// ?implSetSenses@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z
//
// Virtual: switch the perception senses preset selected by `id`. Idle and battle/default install
// the descriptor's respective senses presets and clear the "disabled" mind bit; disabled zeroes
// the senses and sets the bit. Idle and disabled also reset the reported view-percent.
void aiBRAIN::implSetSenses(const dsTSTRING<char> &id)
{
    aiSTATUS::PERC &perc = st.pointee->perc;
    dsSTRID sel(id.CStr(), true);

    if (AI_SETSEN_IDLE.id == sel.id)
    {
        SetupIdleSubSystems();
        mind.SetNormalSelene();
        perc.senses = *aiBRAIN_DESC_IdleSensesPreset(spDesc.pointee);
        st.pointee->mind.val &= ~8;
        st.pointee->perc.viewPrecent = 0;
        _apLog("~AI,Senses~'%s': SetSenses idle", names.nameInst);
    }
    else if (AI_SETSEN_DEFAULT.id == sel.id || AI_SETSEN_BATTLE.id == sel.id)
    {
        mind.ZeroISA();
        mind.SetNormalSelene();
        perc.senses = *aiBRAIN_DESC_BattleSensesPreset(spDesc.pointee);
        st.pointee->mind.val &= ~8;
        _apLog("~AI,Senses~'%s': SetSenses battle(default)", names.nameInst);
    }
    else if (AI_SETSEN_DISABLED1.id == sel.id || AI_SETSEN_DISABLED2.id == sel.id)
    {
        SetupIdleSubSystems();
        mind.ZeroSelene();
        perc.senses = aiSENSES{}; // zeroed preset
        st.pointee->mind.val |= 8;
        st.pointee->perc.viewPrecent = 0;
        _apLog("~AI,Senses~'%s': SetSenses disabled", names.nameInst);
    }
    else
    {
        _apLog("~AI,Senses, Error~'%s': SetSenses('%s') - unrecognized param", names.nameInst, id.CStr());
    }
}
