#include "dsVECTOR.h"

struct dsObjOBJVecMapITEM; // boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>::ITEM (opaque; pointer element)

// dsVECTOR<...objOBJ-map ITEM*,8>::dsVECTOR(apCL) @ 0x82681198
// DEVIATION: decompile flagged "local variable allocation has failed"; reconstructed from the
// generic dsVECTOR default constructor idiom confirmed at every other instantiation in this
// corpus — null storage, zero count/capacity, and copy the call-site cookie.
template<>
dsVECTOR<dsObjOBJVecMapITEM *, 8>::dsVECTOR(const apCL &cl)
    : pData(0), nElem(0), allocated(0), __cl(cl)
{
}
