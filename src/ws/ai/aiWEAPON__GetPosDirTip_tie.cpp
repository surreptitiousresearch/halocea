#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_TIE.h"
#include "../../headers/ws/ai/aiS_VARS.h"
#include "../../headers/ws/ai/aiSTRIKE.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::GetPosDirTip @ 0x832957E0
// ?GetPosDirTip@aiWEAPON@@IBA_NABVaiS_TIE@@AAUm3dV@@1@Z
//
// Wrap a strike tie into a transient aiSTRIKE (resolving its weapon interface) and delegate to the
// strike-keyed muzzle tip query. A null tie yields no tip.
bool aiWEAPON::GetPosDirTip(const aiS_TIE &tie, m3dV &posTip, m3dV &dirTip) const
{
    if (tie.IsNull())
        return false;

    aiSTRIKE strike;
    strike.vars   = tie.vars;
    strike.wpn    = GetIFace(tie);
    strike.desc   = tie.desc;
    strike.weight = 0.0f;
    return GetPosDirTip(strike, posTip, dirTip);
}
