#include "MAP.h"
#include "ds_assert_boundary.h"
#include "dsSTRID.h"
#include <intrin.h> // __umulh (64x64->high64) for the hash reduction below

// ds::MAP<dsSTRID,int,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::GetHash<dsSTRID> @ 0x826217C0
// (apCONFIG::exportMap). Same disasm-exact scheme as the DBG_CINE_ACTOR_INFO-valued instantiation
// (see MAP_dsSTRID_DBG_CINE_ACTOR_INFO__GetHash.cpp): 64-bit sign-extended mix
// h = (int64)a * (int64)key->id + (int64)b, reduced modulo ds::BIG_PRIME (0x55555561) via
// q = high64(h * 0xBFFFFFE5C0000397) >> 30; val = low32(h - q * 0x55555561).
// DEVIATION: an earlier reconstruction did the mix in 32 bits with a fabricated 0x55555540 seed
// and claimed the STRONG_ASSERT was elided; the real body at 0x826217C0 carries the assert. The
// previously recorded address 0x82766688 was wrong (mid-body of an unrelated sort function).
template<>
template<>
unsigned int ds::MAP<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::GetHash(const dsSTRID *key) const
{
    long long h = (long long)(int)a * (long long)(intptr_t)key->id + (long long)(int)b;
    unsigned long long q = __umulh((unsigned long long)h, 0xBFFFFFE5C0000397ULL) >> 30;
    int val = (int)(unsigned int)((unsigned long long)h - q * 1431655777ULL);
    if (!IGNORE_STRONG_ASSERT) {
        if (val < 0 || (unsigned int)val >= 1431655777u)
            ((STRONG_ASSERT_DUMMY *)nullptr)->Crash("val >= 0 && (unsigned) val < ds::BIG_PRIME",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp", 243, empty_string);
    }
    return (unsigned int)val;
}
