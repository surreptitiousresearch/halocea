#include "MAP.h"
#include "ds_boundary.h"
#include "../dbg/DBG_CINE_ACTOR_INFO.h"
#include "../m3d/m3dRND_GEN.h"
#include "dsSTRID.h"

extern m3dRND_GEN m3dRndGen; // boundary — process-wide PRNG used to seed hash-table salts

// ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::MAP @ 0x823B8FD8
// Default-construct an empty map tagged with the caller's allocation call-site cookie `cl`.
// DEVIATION: the decompile flagged "local variable allocation has failed" and modeled `this` as
// raw DWORD-array writes through an uninitialized-looking `v2`/`v3` pair; every offset below was
// re-derived against the DB-verified MAP/dsVECTOR/dsNODE_CACHE/PTR_LIST layouts (all fields land
// exactly on member boundaries) and rewritten as typed field stores instead of `v3[n]` pokes. The
// allocator sub-object's zero-init (nNode/nodeList/freeNodeList) is now expressed as a delegated
// `dsNODE_CACHE_ALLOCATOR(apCL)` member-initializer call (see dsNODE_CACHE_ALLOCATOR.h) rather than
// the raw field pokes an earlier pass on this file used, matching the canonical MAP.h field type.
template<>
ds::MAP<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::MAP(apCL cl)
    : allocator(cl)
{
    this->count = 0;

    this->data.pData     = nullptr;
    this->data.nElem     = 0;
    this->data.allocated = 0;
    this->data.__cl      = cl;

    this->cl = cl;

    // Randomized bucket-count seed / salt pair consumed by ds::HASH::hash<K>(key, a, b) — see
    // MAP_dsSTRID_DBG_CINE_ACTOR_INFO__Find.cpp.
    this->a = ds::FindPrime(m3dRndGen.RndInt() + 10);
    this->b = m3dRndGen.RndInt();
}
