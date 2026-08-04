// aiFEE::IsValid @ 0x8321F300
#include "../../headers/ws/ai/aiFEE.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"

bool aiFEE::IsValid() const
{
    aiWATCHER *watcher;
    return hold
        && (watcher = hold->watcher) != nullptr
        && watcher->GetEntity()
        && !watcher->IsDead();
}
