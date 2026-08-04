#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/msg/msgTYPE_ITER.h"

// aiPLANNER::FindWithOnScreenIdx(int) @ 0x8316ADD0  (virtual)
// Return the first aiBRAIN registered with the message system whose on-screen debug index equals
// `index`, or null when none matches.
// DEVIATION: the decompiler garbles the typed msg-iterator and the descriptor on-screen-index
// read (result->spDesc.pointee[72].__vftable). Modeled at the observed semantics level: iterate
// all aiBRAIN endpoints and compare their on-screen index (inferred aiBRAIN_IFACE vtbl slot).
aiBRAIN_IFACE *aiPLANNER::FindWithOnScreenIdx(int index)
{
    for (msgTYPE_ITER<aiBRAIN> it(msgSystem.Front()); !it.IsDone(); ++it)
    {
        aiBRAIN_IFACE *brain = it.Get();
        if (brain->GetOnScreenIdx() == index)
            return brain;
    }
    return nullptr;
}
