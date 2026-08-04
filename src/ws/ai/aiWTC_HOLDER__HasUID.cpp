#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"

// aiWTC_HOLDER::HasUID(unsigned int) const @ 0x8316E740
// True when the held watcher's unique id equals `uid`.
bool aiWTC_HOLDER::HasUID(unsigned int uid) const
{
    return uid == this->watcher->GetUID();
}
