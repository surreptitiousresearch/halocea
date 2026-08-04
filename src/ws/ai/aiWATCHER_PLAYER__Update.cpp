#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"
#include "../../headers/ws/ai/aiWATCHER_batch_boundaries.h" // usePROCESSOR_GetContextPropList

// aiWATCHER_PLAYER::Update @ 0x83290610
// ?Update@aiWATCHER_PLAYER@@UAA_NM@Z
// Virtual per-frame tick for a player watcher: refresh the cached body coordinate system, translate
// the current STRID FSM flags from the use-processor's context property list (when present), then
// run the base watcher tick.
bool aiWATCHER_PLAYER::Update(float dt)
{
    if (!IsValid())
        return false;

    UpdateBodyCS();
    if (useProcessor)
        TranslateSTRID_FLAGS(&usePROCESSOR_GetContextPropList(useProcessor));

    return aiWATCHER::Update(dt);
}
