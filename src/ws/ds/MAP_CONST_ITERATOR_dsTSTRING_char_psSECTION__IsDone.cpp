#include "MAP.h"
#include "../ps/psSECTION.h"
#include "dsTSTRING.h"

// ds::impl::MAP_CONST_ITERATOR<dsTSTRING<char>,psSECTION,...>::IsDone() @ 0x8270F218
template<>
bool ds::impl::MAP_CONST_ITERATOR<dsTSTRING<char>, psSECTION, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::IsDone() const
{
    return this->item == nullptr;
}
