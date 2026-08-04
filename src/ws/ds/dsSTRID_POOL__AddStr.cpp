#include <new>
#include "dsSTRID_POOL.h"
#include "../os/osLOCK.h"

extern osLOCK dsStrIdLock; // ds global — serialises string interning

// dsSTRID_POOL::AddStr @ 0x8252B6E0
// Intern `str`: if already pooled, return its dsSTRID; otherwise, under the global intern lock
// (double-checked), copy the string into the pool, register key->id in the sorted map, and return
// the new dsSTRID. Returns dsSTRID by value (sret in r3).
// CAVEAT: the decompiler mismodeled the ABI (same sret/this swap as GetIdByStr): r3 is the
// struct-return dsSTRID, r4 is `this` (dsSTRID_POOL*), r5 is `str`. The trailing int parameters it
// invented and its `this->stringMap.a = ...` writes are really stores into the returned dsSTRID.
dsSTRID dsSTRID_POOL::AddStr(const char *str)
{
    dsSTRID result;
    const char *key = str;

    const dsSTRID *found = this->stringMap.Find(&key);
    if (found) {
        result.id = found->id;
        return result;
    }

    // Lazily construct the process-wide intern lock on first miss.
    static bool lockInited = false;
    if (!lockInited) {
        lockInited = true;
        ::new ((void *)&dsStrIdLock) osLOCK("dsStrIdLock"); // + atexit dtor registration (elided)
    }

    dsStrIdLock.Lock(0, 0);
    found = this->stringMap.Find(&key);
    if (found) {
        result.id = found->id;
    } else {
        const char *pooledCopy = this->AllocStr(str);
        int index = this->stringTable.Alloc(pooledCopy);
        const char *stored = this->stringTable.Get(index);
        dsSTRID entry;
        entry.id = stored;
        this->stringMap.Insert(pooledCopy, entry);
        result.id = entry.id;
    }
    dsStrIdLock.Unlock(0, 0);
    return result;
}
