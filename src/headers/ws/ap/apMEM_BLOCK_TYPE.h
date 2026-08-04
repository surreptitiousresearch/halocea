#pragma once
// ws-engine ap: memory-block accounting category. DB-verified (idaapi enum details
// apMEM_BLOCK_TYPE). AP_MEM_BT_NA is the -1 "not-applicable" sentinel; STATIC..RSX are
// the four live buckets that index apMEM_SIZE_STAT::sizes[4] / count[4].

typedef enum apMEM_BLOCK_TYPE {
    AP_MEM_BT_NA        = -1, // vacated / not-applicable slot sentinel
    AP_MEM_BT_STATIC    = 0,
    AP_MEM_BT_ALLOCATOR = 1,
    AP_MEM_BT_SYSTEM    = 2,
    AP_MEM_BT_RSX       = 3,
    AP_MEM_BT_LAST      = 4,
} apMEM_BLOCK_TYPE;
