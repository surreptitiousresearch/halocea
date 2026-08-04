#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"

// aiWAH_SYS::PeekByUID @ 0x83171848
// ?PeekByUID@aiWAH_SYS@@QBAPAVaiWATCHER@@K@Z
// The active watcher whose UID equals `uid`, or null if none is registered. GetUID is the watcher
// virtual at vtbl slot 0x54.
aiWATCHER *aiWAH_SYS::PeekByUID(unsigned int uid) const
{
    int nElem = storage.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiWTC_HOLDER *holder = storage[i];
        if (holder->watcher->GetUID() == uid)
            return holder->watcher;
    }
    return nullptr;
}
