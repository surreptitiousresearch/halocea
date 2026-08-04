#pragma once
#include "VAL_LIST.h"
// ws-engine ds: a pool-recycled shared reference counter (used by ds::dsSHARED_PTR-family smart
// pointers as their control block). DB-verified layout (types_members dsSHARED_COUNTER):
// used@0 (int) — size 4.

typedef struct dsSHARED_COUNTER {
    int used; // 0x00 nonzero while a live dsSHARED_PTR references this slot

    dsSHARED_COUNTER(int initialUsed); // boundary — external to this batch

    // 0x82B8F8A8 — process-wide lazily-constructed pool of recycled dsSHARED_COUNTER slots
    // (the smart-pointer control-block allocator); allocated once via `operator new` + manual
    // field init (usedHead/usedTail/freeHead = -1, empty pool vector) on first call, cached in
    // the static `pool` global thereafter.
    static ds::VAL_LIST<dsSHARED_COUNTER> *GetPool();
} dsSHARED_COUNTER;
