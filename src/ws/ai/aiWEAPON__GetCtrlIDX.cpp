#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"

// aiWEAPON::GetCtrlIDX @ 0x83292B68
// ?GetCtrlIDX@aiWEAPON@@IBA?AV?$aiW_IDX@VpropWPN_AI_USABLE@@@@PBVpropWPN_AI_USABLE@@@Z
//
// Resolve a controller pointer to its weapon-list index by delegating to the wpnWEAPON-keyed
// overload (a null controller resolves through a null weapon).
aiW_IDX<propWPN_AI_USABLE> aiWEAPON::GetCtrlIDX(const propWPN_AI_USABLE *ctrl) const
{
    const wpnWEAPON *wpn = ctrl ? ctrl->weapon() : nullptr;
    return GetCtrlIDX(wpn);
}
