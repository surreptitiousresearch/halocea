#include "dsEVENT_MGR.h"
#include "dsSTRID.h"

// dsEVENT_MGR::RegisterEvent @ 0x8261AA00
// Register (or look up) the event named `name`. Returns the existing handle if the name is already
// registered; otherwise allocates an ITEM_REC, records the interned name, initialises its eight
// per-thread handler chains to -1, inserts name->handle into `itemMap` under the lock, and returns
// the new handle (the item-pool index). The lookup is double-checked around the lock.
// CAVEAT: `name` is a dsSTRID passed by value (r4); the decompiler split it into a pointer + a
// by-value copy. The int handle is returned in r3 (rendered as const char* by the decompiler).
int dsEVENT_MGR::RegisterEvent(dsSTRID name)
{
    const char *keyStr = name.CStr();
    const int *found = this->itemMap.Find(&keyStr);
    if (found)
        return *found;

    this->lock.Lock(0, 0);
    keyStr = name.CStr();
    found = this->itemMap.Find(&keyStr);
    if (found) {
        int handle = *found;
        this->lock.Unlock(0, 0);
        return handle;
    }

    int handle = this->items.Alloc();
    ITEM_REC *rec = &this->items.Get(handle);

    dsSTRID interned(name.CStr(), 0); // re-intern the event name
    rec->name.id = interned.id;

    for (int thread = 0; thread < 8; ++thread)
        rec->threadInfo[thread].idxHandlerChain = -1;

    const char *mapKey = rec->name.CStr();
    this->itemMap.Insert(mapKey, handle);

    this->lock.Unlock(0, 0);
    return handle;
}
