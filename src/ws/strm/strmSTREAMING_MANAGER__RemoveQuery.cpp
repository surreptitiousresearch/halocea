#include "../../headers/ws/strm/strmSTREAMING_MANAGER.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// strmSTREAMING_MANAGER::RemoveQuery @ 0x828F22F0
// Dispatch to the queue matching resource->GetQueueId() (asserting the id is in range first).
bool strmSTREAMING_MANAGER::RemoveQuery(strmRESOURCE *resource)
{
    if (!IGNORE_STRONG_ASSERT &&
        (resource->GetQueueId() < STRM_QUEUE_DVD || resource->GetQueueId() >= STRM_QUEUE_COUNT))
        STRONG_ASSERT_DUMMY().Crash("resource->GetQueueId() >= 0 && resource->GetQueueId() < STRM_QUEUE_COUNT",
            "D:\\Projects\\code\\common\\src.sys\\drv\\strm_resource.cpp", 783, empty_string);

    STRM_QUEUE_ID queueId = resource->GetQueueId();
    return queues.list[queueId].RemoveQuery(resource);
}
