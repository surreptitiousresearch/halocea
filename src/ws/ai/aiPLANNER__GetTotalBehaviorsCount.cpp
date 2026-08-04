#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/msg/msgTYPE_ITER.h"

// aiPLANNER::GetTotalBehaviorsCount() @ 0x8316AD48
// Sum GetBehaviorsCount() over every aiBRAIN registered with the message system.
// DEVIATION: the decompiler garbles the typed msg-iterator + the per-brain vtbl dispatch
// (i[15].TypeID + 10). Modeled at the observed semantics level: iterate all aiBRAIN endpoints via
// msgTYPE_ITER<aiBRAIN> and accumulate their behavior count (inferred aiBRAIN_IFACE vtbl slot).
int aiPLANNER::GetTotalBehaviorsCount()
{
    int total = 0;
    for (msgTYPE_ITER<aiBRAIN> it(msgSystem.Front()); !it.IsDone(); ++it)
        total += it.Get()->GetBehaviorsCount();
    return total;
}
