#include "apDLSTAT_SMALL_IFACE.h"
#include "apMEM_STAT_HASH.h"
#include "apMEM_BLOCK_TYPE.h"
#include "../ds/ds_assert_boundary.h"
#include <string.h>

extern "C" unsigned int dlChunkSize(void *ptr);          // boundary — dlmalloc chunk size
bool check_ptr(void *ptr);                               // boundary — heap pointer validity check
apMEM_ALLOC_INFO *apDlstatSmallRemove(void *memBlock);   // boundary — pull the hash slot for a block

// apDLSTAT_SMALL_IFACE::Remove @ 0x82C4C5B8 — static (SA mangle).
// Recover the allocation record for `memBlock` into `info`. Allocator blocks carry their
// {fileName,fileLine} footer in the tail of the dl chunk; every other block was bucketed into the
// `_apMemStat` hash, so its slot is pulled out. Always returns true.
bool apDLSTAT_SMALL_IFACE::Remove(void *memBlock, bool isAllocator, apMEM_ALLOC_INFO *info)
{
    if (isAllocator)
    {
        if (!IGNORE_STRONG_ASSERT && !check_ptr(memBlock))
            reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
                "check_ptr(memBlock)",
                "D:\\Projects\\code\\common\\src.sys\\ap\\ap_dlstat_small.cpp",
                226,
                empty_string);

        unsigned int chunkSize = dlChunkSize(memBlock);

        // read the {fileName,fileLine} footer (6 bytes) back out of the chunk tail
        struct { const char *fileName; unsigned short fileLine; } footer;
        memcpy(&footer, (char *)memBlock + chunkSize - 6, 6);

        info->memSize  = chunkSize;
        info->type     = AP_MEM_BT_ALLOCATOR;
        info->fileName = footer.fileName;
        info->fileLine = footer.fileLine;
    }
    else
    {
        apMEM_ALLOC_INFO *realInfo = apDlstatSmallRemove(memBlock);
        if (!IGNORE_STRONG_ASSERT && !realInfo)
            reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
                "realInfo",
                "D:\\Projects\\code\\common\\src.sys\\ap\\ap_dlstat_small.cpp",
                237,
                empty_string);

        const char *fileName = realInfo->fileName;
        realInfo->memBlock = nullptr;
        info->fileName = fileName;
        info->memSize  = realInfo->memSize;
        info->memBlock = realInfo->memBlock;
        // Original copies the fileLine(u16)+type(byte)+pad word as one DWORD; split for clarity.
        info->fileLine = realInfo->fileLine;
        info->type     = realInfo->type;
    }

    return true;
}
