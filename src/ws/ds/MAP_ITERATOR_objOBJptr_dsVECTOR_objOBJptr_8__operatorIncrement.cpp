#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only
#include "ds_assert_boundary.h"

// ds::impl::MAP_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>::operator++() @ 0x82687CC8
// Own copy of the base's chain-then-bucket advance walk (identical body to
// MAP_CONST_ITERATOR::operator++, but the compiler emitted a separate copy for the mutable type
// rather than reusing the base — reproduced faithfully rather than delegating, matching the
// disassembly's duplicate code body).
template<>
ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> &
ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::operator++()
{
    typedef ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM ITEM;
    typedef ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> MapType;

    ITEM *current = reinterpret_cast<ITEM *>(this->item);
    if (!current) {
        if (!IGNORE_STRONG_ASSERT)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "!\"Trying to increment iterator that's already done\"",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp", 63, empty_string);
        return *this;
    }

    if (current->next) {
        this->item = current->next;
        return *this;
    }

    MapType *map = reinterpret_cast<MapType *>(this->container);
    ITEM *found = nullptr;
    if (map->data.nElem) {
        int bucket = this->index;
        do {
            ++bucket;
            this->index = bucket;
        } while (bucket < map->data.nElem && !map->data[bucket]);

        if (bucket < map->data.nElem) {
            found = map->data[bucket];
            this->item = found;
            return *this;
        }
    } else if (!IGNORE_STRONG_ASSERT) {
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "!\"Invalid iterator: reference to empty map\"",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_map.hpp", 73, empty_string);
    }

    this->container = nullptr;
    this->index = 0;
    this->item = found; // nullptr — walk exhausted
    return *this;
}
