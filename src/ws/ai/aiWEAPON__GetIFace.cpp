#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/ds/WEAK_PTR.h"

// aiWEAPON::GetIFace @ 0x832944F0
// ?GetIFace@aiWEAPON@@IBAPBVwpnWEAPON@@V?$WEAK_PTR@VpropWPN_AI_USABLE@@@ds@@@Z
//
// Resolve a controller weak-reference (taken by value, so its handle reference is released when
// the parameter goes out of scope) to the weapon interface that controller drives.
const wpnWEAPON *aiWEAPON::GetIFace(ds::WEAK_PTR<propWPN_AI_USABLE> ctrl) const
{
    const wpnWEAPON *result = nullptr;
    if (ctrl.pHandle && ctrl.pHandle->pPtr)
        result = ctrl.pHandle->pPtr->weapon();
    return result;
    // ~WEAK_PTR(ctrl) releases the adopted handle here (inlined in the binary).
}
