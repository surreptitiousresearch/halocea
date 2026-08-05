#include "vidLOCK.h"
#include "../ds/ds_assert_boundary.h"
#include "../../headers/vid_boundary.h"

// 0x82667738
void vidLOCK::Lock(const char *file, int line)
{
    lock.Lock(file, line);

    if (!IGNORE_STRONG_ASSERT && lockDepth && lockThread != osGetCurThreadId())
        ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
            "lockDepth==0 || lockThread==osGetCurThreadId()",
            "D:\\Projects\\code\\common\\src.sys\\drv\\video\\vid.cpp",
            556,
            empty_string);

    ++lockDepth;
    int curThreadId = osGetCurThreadId();
    int newLockDepth = lockDepth;
    lockThread = curThreadId;

    if (newLockDepth == 1)
        vidDRIVER__AcquireThreadOwnership(vidDriver);
}
