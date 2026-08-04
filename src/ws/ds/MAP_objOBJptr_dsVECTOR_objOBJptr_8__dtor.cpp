#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::~MAP @ 0x8268B660
// Return every live node to the allocator, tear down the allocator's owned node blocks, then
// destroy the bucket vector and the allocator's own nodeList vector.
template<>
ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::~MAP()
{
    Clear();
    allocator.Term();
    // allocator.nodeList and data are destroyed automatically by their own dsVECTOR destructors
    // (matches the decompile's explicit dsVECTOR<DUMMY*,8>::~dsVECTOR / dsVECTOR<ITEM*,8>::~dsVECTOR
    // calls — expressed here as ordinary C++ member teardown).
}
