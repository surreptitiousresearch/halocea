#include "MAP.h"
#include "../m3d/m3dRND_GEN.h"
struct msgADDR; // fwd (pointer element only)

extern m3dRND_GEN m3dRndGen; // boundary — process-wide PRNG used to seed hash-table salts

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::MAP @ 0x8255C2D8
// Default-construct an empty map tagged with the caller's allocation call-site cookie `cl`
// (msgSYSTEM::msgSYSTEM constructs its `addrHash` member this way).
// DEVIATION/CAVEAT: the decompile is flagged "local variable allocation has failed". The clean
// parts (count=0, data zeroed, an explicit nested `dsNODE_CACHE<DUMMY>::dsNODE_CACHE(&allocator,
// cl)` call, `this->cl = cl` at offset 0x4C, and a trailing GenHashes() call) are reproduced
// directly via the member-initializer list + body below. Two additional raw stores the decompile
// prints (`data.__cl.file = this`, `data.__cl.line = &cl`) are nonsensical for an apCL value
// (a self-pointer and a stack address are not a {file,line} pair) and are dropped as reg-alloc
// corruption artifacts — `data`'s own apCL is left at dsVECTOR's zero default.
template<>
ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::MAP(apCL cl)
    : a(0), b(0), count(0), data(), allocator(cl)
{
    this->cl = cl;
    GenHashes(); // seeds a/b (FindPrime(rand()+10), rand())
}
