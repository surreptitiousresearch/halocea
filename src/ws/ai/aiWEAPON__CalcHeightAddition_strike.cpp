#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiSTRIKE.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"
#include "../../headers/ws/wpn/atkDSP_INFO.h"

// aiWEAPON::CalcHeightAddition @ 0x83293B78
// ?CalcHeightAddition@aiWEAPON@@UBAMABUm3dV@@ABUaiSTRIKE@@@Z
//
// Public height-addition for an explicit strike candidate: same policy as the tie variant but the
// attack info comes from the strike's own weapon/attack id. Returns 0 when the weapon-state's "no
// aim" bit is set, the attack is invalid, or the accel/splash flags are missing.
float aiWEAPON::CalcHeightAddition(const m3dV &trg, const aiSTRIKE &strike) const
{
    float result = 0.0f;
    atkDSP_INFO info;
    if ((pBrain->st.pointee->wpn.val & 1) == 0
        && strike.wpn->shooter.IsAtkValid(strike.desc->atkID)
        && (strike.wpn->GetAttackInfo(strike.desc->atkID, &info),
            (*(const unsigned char *)&info & 0x40) != 0)
        && (*(const unsigned char *)&info & 0x80)) {
        const wpnS_DESC *desc = strike.desc;
        if (desc->paramsParabolic.heightMax <= 0.000001f
            || strike.vars->parabolicPrediction <= 0.000001f)
            result = CalcHeightAddition(trg, info, desc);
        else
            result = strike.vars->parabolicPrediction;
    }
    return result;
}
