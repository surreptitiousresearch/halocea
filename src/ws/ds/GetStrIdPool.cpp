#include <new>
#include "dsSTRID_POOL.h"
#include "ds_assert_boundary.h"

extern "C" dsSTRID_POOL *dsStrIdPool;      // ds global — lazily-created singleton pool instance
extern "C" const char   *dsStrIdOfEmptyId; // ds global — the interned id of the empty string

// Debug placement operator new the ws build links in. boundary.
void *operator new(size_t size, const char *file, unsigned int line);

// GetStrIdPool @ 0x8252B800
// Lazily-initialized Meyers-style singleton accessor for the process-wide string-id pool: on
// first call, heap-allocate and construct the pool, publish it to the global before doing
// anything else (so the reentrant AddStr/GetStrIdPool calls below see it), intern the shared
// "empty message" string as the pool's first entry, then cache its pooled copy as the canonical
// empty dsSTRID string. Later calls just return the cached global.
dsSTRID_POOL &GetStrIdPool()
{
    if (!dsStrIdPool) {
        void *poolMem = operator new(
            sizeof(dsSTRID_POOL), "D:\\Projects\\code\\common\\src.sys\\ds\\ds_strid.cpp", 0x6C);
        dsSTRID_POOL *newPool = poolMem ? new (poolMem) dsSTRID_POOL() : nullptr;
        dsStrIdPool = newPool;

        // Intern the shared empty-message string so pool slot 0 holds the canonical empty string.
        dsStrIdPool->AddStr(empty_string);

        dsSTRID_POOL &pool = GetStrIdPool(); // dsStrIdPool is now set; recurses once, returns immediately
        dsStrIdOfEmptyId = pool.stringTable.Get(0);
    }
    return *dsStrIdPool;
}
