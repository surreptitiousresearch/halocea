#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only
#include "ds_assert_boundary.h"

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::GetHash<objOBJ*>
// @ 0x82685370
// Raw (unreduced) hash of `*key` via the stateless ds::HASH policy, salted with this map's a/b.
// Strong-asserts the result is < ds::BIG_PRIME (ds_map.hpp:243). Matches the corpus's other
// GetHash<K> instantiations (cf. MAP_int_msgADDRptr__GetHash.cpp): the disassembly inlines
// ds::HASH::hash<objOBJ*>'s body directly here as `(unsigned)key * a + b` reduced modulo
// ds::BIG_PRIME via a magic-number division — that arithmetic belongs to hash<K> itself, not to
// GetHash, so it is left as the boundary call rather than duplicated inline.
template<>
template<>
unsigned int ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::GetHash<objOBJ *>(
    objOBJ *const *key) const
{
    unsigned int val = ds::HASH::hash<objOBJ *>(*key, a, b);
    if (!IGNORE_STRONG_ASSERT && val > ds::BIG_PRIME) {
        // ds_map.hpp:243 — "val >= 0 && (unsigned) val < ds::BIG_PRIME"
        STRONG_ASSERT2_HELPER::asserd<unsigned int>(
            "val >= 0 && (unsigned) val < ds::BIG_PRIME",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp",
            243, "val", val);
    }
    return val;
}
