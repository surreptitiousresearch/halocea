#pragma once
#include "apMEM_ALLOC_INFO.h"
// ws-engine ap: the small-allocation debug statistics hash table. Live allocations are bucketed
// by (memBlock >> 10) & 0x7F into one of 128 rows of 1024 apMEM_ALLOC_INFO slots; free_block[row]
// caches the next-free-slot hint for that row.
// DB-verified layout (types_members apMEM_STAT_HASH):
//   allocInfo@0 (apMEM_ALLOC_INFO[128][1024]), free_block@2097152 (int[128]) — size 2097664.

typedef struct apMEM_STAT_HASH {
    apMEM_ALLOC_INFO allocInfo[128][1024]; // 0x000000 per-row allocation slots
    int              free_block[128];      // 0x200000 next-free-slot hint per row

    // 0x82C4C810 — default ctor: mark every one of the 128*1024 slots vacant (memBlock/memSize/
    // fileName/fileLine cleared, type = AP_MEM_BT_NA sentinel) and zero every row's free-slot hint.
    apMEM_STAT_HASH();
} apMEM_STAT_HASH;

// ?_apMemStat@@3PAVapMEM_STAT_HASH@@A — global pointer to the live table.
extern apMEM_STAT_HASH *_apMemStat;
