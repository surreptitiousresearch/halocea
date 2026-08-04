#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // aiBODY_IFACE / ctrlPROP_LIST

// aiWATCHER_BRAIN::Update(float) @ 0x8316EF90  (virtual)
// Run the base watcher tick; on failure clear the FSM property flags and report inactive. On
// success, rebuild the FSM property flags from the tracked brain body's cached property set and
// refresh the cached reticle. (The base debug assert brain && brain->body && brain->body->wpn is
// omitted here — it is a strong-assert-only sanity check.)
bool aiWATCHER_BRAIN::Update(float dt)
{
    // aiWATCHER is the offset-0 base of aiWATCHER_BRAIN (modeled as an opaque block in the header).
    aiWATCHER *base = reinterpret_cast<aiWATCHER *>(this);

    if (!base->Update(dt))
    {
        base->fsmPropFlags.val = 0;
        return false;
    }

    aiBRAIN *brain = this->brain.pHandle ? this->brain.pHandle->pPtr : nullptr;
    const ctrlPROP_LIST *propCache = brain->body.pointee->GetRefPropCache();
    base->TranslateSTRID_FLAGS(propCache);
    this->UpdateReticleData();
    return true;
}
