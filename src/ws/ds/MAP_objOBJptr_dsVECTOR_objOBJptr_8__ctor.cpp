#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only
#include "../m3d/m3dRND_GEN.h"

extern m3dRND_GEN m3dRndGen; // boundary — process-wide PRNG used to seed hash-table salts

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::MAP @ 0x8268AA30
// Default-construct an empty map tagged with the caller's allocation call-site cookie `cl`.
// DEVIATION: the decompile flagged "local variable allocation has failed" and modeled `this` as
// raw DWORD-array writes through an uninitialized-looking `v2`/`v3` pair; every offset below was
// re-derived against the DB-verified MAP/dsVECTOR/dsNODE_CACHE_ALLOCATOR layouts (all fields land
// exactly on member boundaries) and rewritten as typed field stores instead of `v3[n]` pokes —
// matching the pattern already confirmed for the msgADDR-map and dsSTRID-map instantiations.
template<>
// allocator has no default ctor (only apCL ctor); construct it here — the body's manual
// field stores below then reproduce the decompiled init and are layout-equivalent.
ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::MAP(apCL cl)
    : allocator(cl)
{
    this->count = 0;

    this->data.pData     = nullptr;
    this->data.nElem     = 0;
    this->data.allocated = 0;
    this->data.__cl      = cl;

    this->allocator.nNode = 0;
    this->allocator.nodeList.pData     = nullptr;
    this->allocator.nodeList.nElem     = 0;
    this->allocator.nodeList.allocated = 0;
    this->allocator.nodeList.__cl      = cl;
    this->allocator.freeNodeList.pHead = nullptr;
    this->allocator.freeNodeList.pTail = nullptr;
    this->allocator.freeNodeList.length = 0;
    this->allocator.__cl = cl;

    this->cl = cl;

    // Randomized bucket-count seed / salt pair consumed by GetHash<K> (see
    // MAP_objOBJptr_dsVECTOR_objOBJptr_8__GetHash.cpp).
    this->a = ds::FindPrime(m3dRndGen.RndInt() + 10);
    this->b = m3dRndGen.RndInt();
}
