#include "../../headers/ws/strm/strmSTREAMING_QUEUE.h"
#include "../../headers/ws/strm/strm_boundary.h"
#include "../../headers/ws/ds/dsCMP.h"

// strmSTREAMING_QUEUE::RemoveQuery @ 0x828F2048
// Under `lock`: drop the resource's queue entry if present and mark it RES_STATE_CANCEL, then
// (outside the lock) spin on osSleep(1) while a load for it is still in flight. Always succeeds.
bool strmSTREAMING_QUEUE::RemoveQuery(strmRESOURCE *resource)
{
    lock.Lock(nullptr, 0);

    dsCMP cmp;
    int idx = queries.Find<dsCMP, strmRESOURCE *>(resource, cmp, 0);
    if (idx >= 0) {
        queries.Erase(idx, 1);
        resource->SetState(strmRESOURCE::RES_STATE_CANCEL);
    }

    lock.Unlock(nullptr, 0);

    while (resource->state == strmRESOURCE::RES_STATE_LOADING)
        osSleep(1);

    return true;
}
