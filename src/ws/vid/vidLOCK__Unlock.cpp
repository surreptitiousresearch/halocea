#include "vidLOCK.h"
#include "../ds/ds_assert_boundary.h"
#include "../../headers/vid_boundary.h"

// 0x826677F0
void vidLOCK::Unlock(const char *file, int line)
{
    if (!IGNORE_STRONG_ASSERT && lockDepth <= 0 && lockThread != osGetCurThreadId())
        ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
            "lockDepth>0 || lockThread==osGetCurThreadId()",
            "D:\\Projects\\code\\common\\src.sys\\drv\\video\\vid.cpp",
            566,
            empty_string);

    if (lockDepth == 1)
    {
        vidDRIVER__ReleaseThreadOwnership(vidDriver);
        lockThread = -1;
    }

    --lockDepth;
    lock.Unlock(file, line);
}
