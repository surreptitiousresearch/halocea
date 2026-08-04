#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::GetPosDirTip @ 0x83292CD8
// ?GetPosDirTip@aiWEAPON@@IBA_NPBVpropWPN_AI_USABLE@@AAUm3dV@@1@Z
//
// Muzzle tip pos/dir for a specific controller: resolve the controller to its weapon index, then
// delegate to the index-keyed tip query.
bool aiWEAPON::GetPosDirTip(const propWPN_AI_USABLE *ctrl, m3dV &posTip, m3dV &dirTip) const
{
    const wpnWEAPON *wpn = ctrl ? ctrl->weapon() : nullptr;
    aiW_IDX<propWPN_AI_USABLE> widx = GetCtrlIDX(wpn);
    return GetPosDirTip(widx, posTip, dirTip);
}
