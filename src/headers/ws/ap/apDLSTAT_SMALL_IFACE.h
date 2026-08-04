#pragma once
// ws-engine ap (platform): static interface that owns the small dlmalloc statistics hash
// buffer. Stateless — all state lives in the file-scope `_apMemStat` global.
#include "apMEM_ALLOC_INFO.h"
#include "apMEM_BLOCK_TYPE.h"

struct apMEM_STAT_HASH; // ap subsystem statistics hash table (defined in apMEM_STAT_HASH.h)

typedef struct apDLSTAT_SMALL_IFACE {
    // 0x82C4C878 — allocate the memstat hash buffer, construct it, publish it to `_apMemStat`,
    // and register it with the memory debug tracker.
    static void Start();

    // 0x82C4C328 — record an allocation. For AP_MEM_BT_ALLOCATOR blocks the {fileName,fileLine}
    // footer is stashed in the tail of the dl chunk; every other bucket is bucketed into the
    // `_apMemStat` hash table (row = (memBlock>>10)&0x7F) at the next free slot. Returns 1 on
    // success, 0 if the row is full (strong-assert path). Static (SA mangle).
    static bool Add(void *memBlock, unsigned int memSize, const char *fileName,
                    unsigned int fileLine, apMEM_BLOCK_TYPE type);

    // 0x82C4C5B8 — recover the allocation record for `memBlock` into `info`. For allocator blocks
    // the footer is read back out of the dl chunk tail; otherwise the matching `_apMemStat` slot is
    // removed. Returns 1. Static (SA mangle).
    static bool Remove(void *memBlock, bool isAllocator, apMEM_ALLOC_INFO *info);
} apDLSTAT_SMALL_IFACE;
