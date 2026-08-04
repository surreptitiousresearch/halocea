#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_VARS.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::GetPosDirTip @ 0x83295468
// ?GetPosDirTip@aiWEAPON@@IBA_NABUaiSTRIKE@@AAUm3dV@@1@Z
//
// Muzzle tip for a strike: serve the cached pos/dir when the strike's vars cache is valid; otherwise
// resolve the aim-check type (querying the controller when the descriptor defers, mode 4) and
// dispatch to the index-keyed tip query.
bool aiWEAPON::GetPosDirTip(const aiSTRIKE &strike, m3dV &posTip, m3dV &dirTip) const
{
    aiS_VARS *vars = strike.vars;
    if (vars->isCacheValid) {
        posTip = vars->posTip;
        dirTip = vars->dirTip;
        return true;
    }

    aimCHECK_TYPE type = (aimCHECK_TYPE)strike.desc->typeIsAimed;
    if (type == 4)
        type = GetAimCheck(vars->tie.idxCtrl);

    aiW_IDX<propWPN_AI_USABLE> w;
    w.idx = strike.vars->tie.idxCtrl.idx;
    return GetPosDirTip(w, type, posTip, dirTip);
}
