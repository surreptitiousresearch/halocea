#pragma once
// ws-engine ap: per-category memory accounting. Tracks a running total plus a
// high-water mark for both byte size and live-allocation count, broken down by
// apMEM_BLOCK_TYPE bucket (indices 0..3).
// DB-verified layout (types_members apMEM_SIZE_STAT):
//   sizeTotal@0, countTotal@4, sizeTotalMax@8, countTotalMax@12,
//   sizes@16 (int[4]), count@32 (int[4]) — size 48.
#include "apMEM_BLOCK_TYPE.h"

typedef struct apMEM_SIZE_STAT {
    int sizeTotal;     // 0x00 total live bytes across all buckets
    int countTotal;    // 0x04 total live allocations across all buckets
    int sizeTotalMax;  // 0x08 peak total bytes
    int countTotalMax; // 0x0C peak total allocation count
    int sizes[4];      // 0x10 live bytes per apMEM_BLOCK_TYPE bucket
    int count[4];      // 0x20 live allocation count per bucket

    // Account for a new allocation of `size` bytes in bucket `type`.
    void Add(int size, apMEM_BLOCK_TYPE type);

    // Reverse an allocation of `size` bytes in bucket `type`.
    void Remove(int size, apMEM_BLOCK_TYPE type);

    // 0x826C0C38 — Meyers singleton accessor (function-local static instance).
    static apMEM_SIZE_STAT &Instance();
} apMEM_SIZE_STAT;
