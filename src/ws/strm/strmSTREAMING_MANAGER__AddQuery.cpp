#include "../../headers/ws/strm/strmSTREAMING_MANAGER.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// strmSTREAMING_MANAGER::AddQuery @ 0x828F3A60
// Under `lockAdd`, dispatch to the queue matching resource->GetQueueId().
bool strmSTREAMING_MANAGER::AddQuery(strmRESOURCE *resource)
{
    lockAdd.Lock(nullptr, 0);

    STRM_QUEUE_ID queueId = resource->GetQueueId();
    if (!IGNORE_STRONG_ASSERT && (unsigned int)queueId >= STRM_QUEUE_COUNT)
        STRONG_ASSERT_DUMMY().Crash("queueId >= 0 && queueId < STRM_QUEUE_COUNT",
            "D:\\Projects\\code\\common\\src.sys\\drv\\strm_resource.cpp", 769, dsStrongAssertMessage);

    bool result = queues.list[queueId].AddQuery(resource);
    lockAdd.Unlock(nullptr, 0);
    return result;
}
