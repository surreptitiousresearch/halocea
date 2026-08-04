#pragma once
// ws-engine ap: per-allocation-type dlmalloc statistics record.
// DB-verified layout (types_members apMEM_DL_STAT): type@0, size@4 — size 8.

// ap memory allocation-type enum (int-backed). DB-verified (types_enum_values).
typedef enum apMEM_DL_ALLOC_TYPE {
    AP_DLSTAT_DL_NA    = -1, // vacated / not-applicable slot sentinel
    AP_DLSTAT_DL_FREE  = 0,
    AP_DLSTAT_DL_ALLOC = 1,
    AP_DLSTAT_DL_LAST  = 2,
} apMEM_DL_ALLOC_TYPE;

typedef struct apMEM_DL_STAT {
    apMEM_DL_ALLOC_TYPE type; // 0x00 allocation category
    int                 size; // 0x04 byte count for this category
} apMEM_DL_STAT;
