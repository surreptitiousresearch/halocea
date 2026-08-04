#include "../../headers/ws/strm/strmSTREAMING_QUEUE.h"
#include "../../headers/ws/ds/dsCMP.h"

// strmSTREAMING_QUEUE::AddQuery @ 0x828F38C8
// Under `lock`: bail out (unlock, return false) if the resource is already RES_STATE_LOADING.
// Otherwise drop any existing queue entry for it, and only if OnAddQuery() accepts, push it,
// mark it RES_STATE_IN_QUERY, and (re)start the worker thread.
bool strmSTREAMING_QUEUE::AddQuery(strmRESOURCE *resource)
{
    lock.Lock(nullptr, 0);

    if (resource->state == strmRESOURCE::RES_STATE_LOADING) {
        lock.Unlock(nullptr, 0);
        return false;
    }

    dsCMP cmp;
    int existingIdx = queries.Find<dsCMP, strmRESOURCE *>(resource, cmp, 0);
    if (existingIdx >= 0)
        queries.Erase(existingIdx, 1);

    // DEVIATION: the binary's return value is `(unsigned)existingIdx >> 31` — true only when the
    // resource had NOT already been queued (existingIdx was -1 before the erase above). This is
    // the same "was this a fresh add" signal, just computed before OnAddQuery() is even checked.
    bool wasNotQueued = existingIdx < 0;

    if (!resource->OnAddQuery()) {
        lock.Unlock(nullptr, 0);
        return false;
    }

    queries.PushBack(resource);
    resource->SetState(strmRESOURCE::RES_STATE_IN_QUERY);
    CreateThread();
    EnableStrmThread(true);

    lock.Unlock(nullptr, 0);
    return wasNotQueued;
}
