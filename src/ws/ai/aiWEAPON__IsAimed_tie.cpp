#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_TIE.h"
#include "../../headers/ws/ai/aiS_VARS.h"
#include "../../headers/ws/ai/aiSTRIKE.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::IsAimed @ 0x83295FF8
// ?IsAimed@aiWEAPON@@IBA_NABUm3dV@@ABVaiS_TIE@@@Z
//
// Wrap a strike tie into a transient aiSTRIKE and delegate to the strike-keyed aim test. A null tie
// is never aimed.
bool aiWEAPON::IsAimed(const m3dV &posTarget, const aiS_TIE &tie) const
{
    if (tie.IsNull())
        return false;

    aiSTRIKE strike;
    strike.vars   = tie.vars;
    strike.wpn    = GetIFace(tie);
    strike.desc   = tie.desc;
    strike.weight = 0.0f;
    return IsAimed(posTarget, strike);
}
