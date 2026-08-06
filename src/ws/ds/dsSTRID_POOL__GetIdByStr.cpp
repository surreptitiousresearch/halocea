#include "dsSTRID_POOL.h"

extern "C" const char *dsStrIdOfEmptyId; // ds global — the interned id of the empty string

// dsSTRID_POOL::GetIdByStr @ 0x8252A998
// CAVEAT: the decompiler mismodeled the ABI. Verified against the prologue
// (mr r31,r3; mr r3,r4; bl Find; stw r11,0(r31); mr r3,r31): r3 is the struct-return
// pointer for the returned dsSTRID, r4 is `this` (const dsSTRID_POOL*), r5 is `str`.
// The 12 trailing int parameters the decompiler invented are register-guess artifacts.
// Reconstructed as the intended const method returning dsSTRID by value.
dsSTRID dsSTRID_POOL::GetIdByStr(const char *str) const
{
    const char *key = str;
    // stringMap is dsLF_SORTED_MAP<const char*,...>; Find takes a pointer to the key.
    const dsSTRID *found = this->stringMap.Find(&key);

    dsSTRID id;
    id.id = found ? found->id : dsStrIdOfEmptyId;
    return id;
}
