#include "MAP.h"
#include "ds_assert_boundary.h"
struct msgADDR; // fwd (pointer element only)

// ds::impl::MAP_ITERATOR<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::value @ 0x825575B8
// Pointer to the matched node's value slot; strong-asserts `item != 0` first (ds_map.hpp:169).
template<>
msgADDR **ds::impl::MAP_ITERATOR<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::value() const
{
    if (!IGNORE_STRONG_ASSERT && !item) {
        STRONG_ASSERT_DUMMY().Crash(
            "item != 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp",
            169,
            empty_string);
    }
    return &reinterpret_cast<ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM *>(item)->value;
}
