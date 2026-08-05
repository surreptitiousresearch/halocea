#include "apDLSTAT_SMALL_IFACE.h"
#include "apMEM_STAT_HASH.h"
#include "../ds/ds_assert_boundary.h"
#include <string.h>

extern "C" unsigned int dlChunkSize(void *ptr); // boundary — dlmalloc chunk size

// apDLSTAT_SMALL_IFACE::Add @ 0x82C4C328 — static (SA mangle).
// Record an allocation. AP_MEM_BT_ALLOCATOR blocks stash their {fileName,fileLine} footer in the
// tail of the dl chunk. Every other block is bucketed into the `_apMemStat` hash: row =
// (memBlock >> 10) & 0x7F, placed at the row's next free slot (using free_block[row] as a hint,
// falling back to a full row scan). Returns true on success, false when the row is full.
bool apDLSTAT_SMALL_IFACE::Add(void *memBlock, unsigned int memSize, const char *fileName,
                               unsigned int fileLine, apMEM_BLOCK_TYPE type)
{
    if (type == AP_MEM_BT_ALLOCATOR)
    {
        unsigned int chunkSize = dlChunkSize(memBlock);
        if (!IGNORE_STRONG_ASSERT && memSize > chunkSize)
            reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
                "memSize <= chunkSize",
                "D:\\Projects\\code\\common\\src.sys\\ap\\ap_dlstat_small.cpp",
                128,
                empty_string);

        // write the {fileName,fileLine} footer (6 bytes) into the chunk tail
        struct { const char *fileName; unsigned short fileLine; } footer;
        footer.fileName = fileName;
        footer.fileLine = (unsigned short)fileLine;
        memcpy((char *)memBlock + chunkSize - 6, &footer, 6);
        return true;
    }

    unsigned int block = (unsigned int)(unsigned long)memBlock;
    int row = (block >> 10) & 0x7F;
    apMEM_ALLOC_INFO *rowSlots = _apMemStat->allocInfo[row];

    int slot = _apMemStat->free_block[row];   // next-free-slot hint
    bool needFullScan = (slot >= 1024);
    if (!needFullScan)
    {
        // scan forward from the hint for a vacant slot (memBlock == nullptr)
        while (rowSlots[slot].memBlock)
        {
            if (++slot >= 1024)
            {
                needFullScan = true;
                break;
            }
        }
    }
    if (needFullScan)
    {
        // hint exhausted: linear-scan the whole row from the start
        slot = 0;
        while (rowSlots[slot].memBlock)
        {
            if (++slot >= 1024)
            {
                if (IGNORE_STRONG_ASSERT)
                    return false;
                reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
                    "!\"Too many allocates! Debug stats array is full. Reserve more memory\"",
                    "D:\\Projects\\code\\common\\src.sys\\ap\\ap_dlstat_small.cpp",
                    162,
                    empty_string);
                return false;
            }
        }
    }

    apMEM_ALLOC_INFO *dst = &rowSlots[slot];
    if (dst->memBlock)   // slot unexpectedly occupied
    {
        if (IGNORE_STRONG_ASSERT)
            return false;
        reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
            "0",
            "D:\\Projects\\code\\common\\src.sys\\ap\\ap_dlstat_small.cpp",
            168,
            empty_string);
        return false;
    }

    _apMemStat->free_block[row] = slot + 1;
    dst->memBlock = memBlock;
    dst->memSize  = memSize;
    dst->fileName = fileName;
    dst->fileLine = (unsigned short)fileLine;
    dst->type     = (char)type;
    return true;
}
