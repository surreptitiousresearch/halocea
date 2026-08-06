#include "../headers/ws/vid/vidDEFRAG_POOL_HCEX_X360.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

// vid subsystem boundary — physical/GPU memory allocator (see also strmVID_MEM_BLOCK::Init).
// mangled ?vidAllocPhysMem@@YAPAXKKKPAHK@Z: arg3 (actualSize) is int* (PAH).
extern void *vidAllocPhysMem(unsigned long size, unsigned long align, unsigned long flags,
                              int *actualSize, unsigned long extra);

// vidDEFRAG_POOL_HCEX_X360::AllocBuf @ 0x823F13D4
// Always requests the whole 16 MB (0x1000000) arena in one shot; `bufSize` is unconditionally
// written 0x1000000 regardless of `minSize`. STRONG_ASSERTs that the caller's `minSize` actually
// fits inside that arena before handing back the raw physical-memory pointer.
void *vidDEFRAG_POOL_HCEX_X360::AllocBuf(int minSize, int *bufSize)
{
    *bufSize = 0x1000000;

    if (!IGNORE_STRONG_ASSERT && minSize > 0x1000000)
        STRONG_ASSERT_DUMMY().Crash(
            "minSize <= bufSize",
            "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp",
            131,
            empty_string);

    return vidAllocPhysMem(*bufSize, 0, 4, nullptr, 0);
}
