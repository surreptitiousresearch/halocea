#include "dsSTRID.h"
#include "dsSTRID_POOL.h"

extern dsSTRID_POOL &GetStrIdPool(); // ds global accessor — the process-wide dsStrIdPool singleton

// dsSTRID::dsSTRID(const char*, bool) @ 0x8252B898
// Intern/hash `str` via the global string-id pool: an empty string adopts the pool's reserved
// empty-string slot (stringTable index 0) directly; otherwise look it up (existOnly) or
// intern-and-look-up (!existOnly).
dsSTRID::dsSTRID(const char *str, bool existOnly)
{
    dsSTRID_POOL &pool = GetStrIdPool();
    if (*str) {
        dsSTRID interned = existOnly ? pool.GetIdByStr(str) : pool.AddStr(str);
        this->id = interned.id;
    } else {
        // CAVEAT: decompiler read this as `(dsSTRID*)pool.stringTable.Get(0)` then `->id` --
        // dsPOOL<const char*,...>::Get(index) returns T& (const char*&), and dsSTRID's single
        // `id` member aliases that same layout, so this collapses to reading the pooled
        // const char* directly.
        this->id = pool.stringTable.Get(0);
    }
}
