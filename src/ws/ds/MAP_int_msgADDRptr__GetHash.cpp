#include "MAP.h"
#include "ds_assert_boundary.h"
struct msgADDR; // fwd (pointer element only)

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::GetHash<int> @ 0x82558E90
// Raw (unreduced) hash of `*key` via the stateless ds::HASH policy, salted with this map's a/b.
// Strong-asserts the result is < ds::BIG_PRIME (ds_map.hpp:243) — the assert path is reproduced
// faithfully as a boundary call (STRONG_ASSERT2_HELPER::asserd is the diagnostic formatter, not
// re-derived here).
template<>
template<>
unsigned int ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::GetHash<int>(const int *key) const
{
    unsigned int val = ds::HASH::hash<int>(*key, a, b);
    if (!IGNORE_STRONG_ASSERT && val > ds::BIG_PRIME) {
        // ds_map.hpp:243 — "val >= 0 && (unsigned) val < ds::BIG_PRIME"
        STRONG_ASSERT2_HELPER::asserd<unsigned int>(
            "val >= 0 && (unsigned) val < ds::BIG_PRIME",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp",
            243, "val", val);
    }
    return val;
}
