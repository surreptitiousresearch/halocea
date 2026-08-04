#include "MAP.h"
#include "../ps/psSECTION.h"
#include "../m3d/m3dRND_GEN.h"
#include "dsTSTRING.h"

extern m3dRND_GEN m3dRndGen; // boundary — process-wide PRNG used to seed hash-table salts

// ds::MAP<dsTSTRING<char>,psSECTION,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::MAP @ 0x827121E0
// Identical shape to MAP_dsSTRID_DBG_CINE_ACTOR_INFO__ctor.cpp — see that file for the derivation
// of the field offsets from the raw `v3[n]` decompile (this instantiation's ctor is byte-identical
// modulo the base address, confirming the layout is generic across K/V) and for the delegated
// `allocator(cl)` member-initializer note.
template<>
ds::MAP<dsTSTRING<char>, psSECTION, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::MAP(apCL cl)
    : allocator(cl)
{
    this->count = 0;

    this->data.pData     = nullptr;
    this->data.nElem     = 0;
    this->data.allocated = 0;
    this->data.__cl      = cl;

    this->cl = cl;

    this->a = ds::FindPrime(m3dRndGen.RndInt() + 10);
    this->b = m3dRndGen.RndInt();
}
