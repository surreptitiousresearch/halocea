#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"

// aiWEAPON::IsCtrlActive @ 0x83295058
// ?IsCtrlActive@aiWEAPON@@IBA_NPBVpropWPN_AI_USABLE@@@Z
//
// Controller-keyed overload: resolve the controller to its weapon interface and defer to the
// interface-keyed active check (a null controller resolves through a null interface).
bool aiWEAPON::IsCtrlActive(const propWPN_AI_USABLE *ctrl) const
{
    const wpnWEAPON *iface = ctrl ? ctrl->weapon() : nullptr;
    return IsCtrlActive(iface);
}
