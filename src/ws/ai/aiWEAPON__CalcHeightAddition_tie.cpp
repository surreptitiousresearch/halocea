#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/wpn/atkDSP_INFO.h"

// aiWEAPON::CalcHeightAddition @ 0x83293A90
// ?CalcHeightAddition@aiWEAPON@@UBAMABUm3dV@@@Z
//
// Public height-addition for the currently-selected strike: return the cached parabolic prediction
// when it is valid, otherwise recompute it from the selected strike's attack info. Returns 0 when
// no valid strike/attack is selected or the weapon-state's "no aim" bit is set.
float aiWEAPON::CalcHeightAddition(const m3dV &trg) const
{
    float result = 0.0f;
    const wpnS_DESC *desc;
    atkDSP_INFO info;
    if (!tieSelected.IsNull()
        && (desc = tieSelected.desc) != nullptr
        && (pBrain->st.pointee->wpn.val & 1) == 0
        && GetAtkInfo(&tieSelected, &info)
        && (*(const unsigned char *)&info & 0x40)
        && (*(const unsigned char *)&info & 0x80)) {
        aiS_VARS *vars = tieSelected.vars;
        if (desc->paramsParabolic.heightMax <= 0.000001f
            || !vars
            || vars->parabolicPrediction <= 0.000001f)
            result = CalcHeightAddition(trg, info, desc);
        else
            result = vars->parabolicPrediction;
    }
    return result;
}
