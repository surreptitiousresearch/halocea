#pragma once
#include "dsSTRID.h"
#include "dsLF_SORTED_MAP.h"
#include "dsPOOL.h"
#include "dsSTR_HASH.h"
#include "../../apCL.h"
// ws-engine ds: interning pool that maps strings to dsSTRID ids via a lock-free sorted map.
// DB-verified layout (types_members dsSTRID_POOL):
//   stringMap@0 (36), stringTable@36 (dsPOOL<char const*,1024,512>, 2064),
//   idxStorage@2100, posStorage@2104, strStorage@2108 (char*[256]), _cl@3132 — size 3140.

struct dsSTR_CMP; // boundary — const char* ordering policy for the sorted map (see dsSTR_HASH.h peers)

typedef struct dsSTRID_POOL {
    dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP> stringMap;      // 0x0000 (36)
    dsPOOL<const char *, 1024, 512>                              stringTable;    // 0x0024 (2064)
    int                   idxStorage;     // 0x0834
    int                   posStorage;     // 0x0838
    char                 *strStorage[256];// 0x083C
    apCL                  _cl;            // 0x0C3C

    // 0x8252B000 — construct an empty pool: default-init the sorted map (registering it with the
    // erase manager), zero the string-block bookkeeping and slot table, then presize the map's hash
    // table to a prime >= 4096 buckets (all null).
    dsSTRID_POOL();

    // 0x8252A998 — return the interned id for str, or the empty id when not interned.
    dsSTRID GetIdByStr(const char *str) const;

    // 0x8252B6E0 — intern `str`: return its existing dsSTRID, or (under a global lock, double-
    // checked) copy the string into the pool, register it in the sorted map, and return the new
    // dsSTRID. Returns dsSTRID by value (sret).
    dsSTRID AddStr(const char *str);

    // 0x825281D8 — copy `str` (measured with strlen) into the pool's fixed 8 KiB string blocks,
    // advancing to a fresh block when the current one lacks room (lazily malloc'ing blocks), and
    // return the stable pointer to the interned copy. Protected.
protected:
    const char *AllocStr(const char *str);
public:
} dsSTRID_POOL;
