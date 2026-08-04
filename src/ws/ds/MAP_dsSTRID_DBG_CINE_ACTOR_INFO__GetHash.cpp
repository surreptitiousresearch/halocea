#include "MAP.h"
#include "../dbg/DBG_CINE_ACTOR_INFO.h"
#include "ds_assert_boundary.h"
#include "dsSTRID.h"
#include <intrin.h> // __umulh (64x64->high64) for the hash reduction below

// ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::GetHash<dsSTRID>
// @ 0x823B58A8
// h = (int64)a * (int64)key->id + (int64)b (all operands sign-extended to 64-bit, full 64-bit
// product per the mulld/extsw sequence), then reduced modulo ds::BIG_PRIME (0x55555561) via the
// magic-multiply idiom: q = high64(h * 0xBFFFFFE5C0000397) >> 30; val = low32(h - q * 0x55555561).
// STRONG_ASSERT-checks val >= 0 && (unsigned)val < ds::BIG_PRIME before returning it
// (Find/InsertNew further reduce it modulo data.nElem).
// DEVIATION: an earlier reconstruction did the mix in 32 bits with a fabricated 0x55555540 low-word
// seed — that was a misread of the decompiler's HIDWORD fusion; this form is disasm-exact.
template<>
template<>
unsigned int ds::MAP<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::GetHash(const dsSTRID *key) const
{
    long long h = (long long)(int)a * (long long)(int)key->id + (long long)(int)b;
    unsigned long long q = __umulh((unsigned long long)h, 0xBFFFFFE5C0000397ULL) >> 30;
    int val = (int)(unsigned int)((unsigned long long)h - q * 1431655777ULL);
    if (!IGNORE_STRONG_ASSERT) {
        if (val < 0 || (unsigned int)val >= 1431655777u)
            ((STRONG_ASSERT_DUMMY *)nullptr)->Crash("val >= 0 && (unsigned) val < ds::BIG_PRIME",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp", 243, dsStrongAssertMessage);
    }
    return (unsigned int)val;
}
