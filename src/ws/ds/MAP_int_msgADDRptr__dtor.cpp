#include "MAP.h"
struct msgADDR; // fwd (pointer element only)

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::~MAP @ 0x8255CBB8
// Return every live node to the allocator, tear down the allocator's owned node blocks, then
// destroy the bucket vector and the allocator's own nodeList vector.
template<>
ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::~MAP()
{
    Clear();
    allocator.Term();
    // allocator.nodeList and data are destroyed automatically by their own dsVECTOR destructors
    // (matches the decompile's explicit dsVECTOR<DUMMY*,8>::~dsVECTOR / dsVECTOR<ITEM*,8>::~dsVECTOR
    // calls — here expressed as ordinary C++ member teardown).
}
