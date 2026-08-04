#pragma once
#include "../ds/dsSTRID.h"
#include "dbgVAR_boundary.h"
// ws-engine wb (watch/debug): wraps a named debug variable binding.
// DB-verified layout (types_members wb::dbgVARWrapper):
//   name@0 (dsSTRID), defaultVal@4 (bool), dbgVar@8 (dbgVAR_SIMPLE<bool,1>*) — size 12.

namespace wb {

typedef struct dbgVARWrapper {
    dsSTRID name;                      // 0x00 interned variable name
    bool    defaultVal;                // 0x04 default value
    unsigned char _pad0[3]; /* db-verified padding */
    // 3 bytes padding
    dbgVAR_SIMPLE<bool, 1> *dbgVar;    // 0x08 lazily created by GetValue(); owned (see ~dbgVARWrapper)

    // ??0dbgVARWrapper@wb@@QAA@PBD_N@Z @ 0x8269DA30 — boundary (not one of this batch's
    // targets, but same-class and load-bearing for RegisterPage/PushBack): sets name via
    // dsSTRID(_name, false), copies _defaultVal, and leaves dbgVar null (not created eagerly).
    dbgVARWrapper(const char *_name, bool _defaultVal); // boundary

    // ??0dbgVARWrapper@wb@@QAA@ABV01@@Z @ 0x8269DA80 — boundary: copies name/defaultVal from
    // `other` but always leaves dbgVar null (the lazily-created dbgVAR_SIMPLE is NOT shared or
    // deep-copied — every copy re-lazily-creates its own on first GetValue()).
    dbgVARWrapper(const dbgVARWrapper &other); // boundary

    // ??1dbgVARWrapper@wb@@QAA@XZ @ 0x8269DAD0 — boundary: if dbgVar was ever created, invokes
    // its vector-deleting destructor slot with freeMemory=1 (deletes it) and nulls the pointer.
    ~dbgVARWrapper(); // boundary

    // ?GetValue@dbgVARWrapper@wb@@QAA_NXZ @ 0x8269DDB0 — lazily creates dbgVar on first call
    // (heap dbgVAR_SIMPLE<bool,1>, named via ds::ToString(name), seeded from defaultVal), then
    // returns its live current value. Non-const per the QAA mangle (mutates dbgVar).
    bool GetValue();
} dbgVARWrapper;

} // namespace wb
