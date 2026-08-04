#include "dsVECTOR.h"
#include "dsObjOBJVecMapDUMMY.h"

// dsVECTOR<...objOBJ-map DUMMY*,8>::dsVECTOR(apCL) @ 0x82681330
// DEVIATION: decompile flagged "local variable allocation has failed"; reconstructed from the
// generic dsVECTOR default constructor idiom confirmed at every other instantiation in this
// corpus — null storage, zero count/capacity, and copy the call-site cookie.
template<>
dsVECTOR<dsObjOBJVecMapDUMMY *, 8>::dsVECTOR(const apCL &cl)
    : pData(0), nElem(0), allocated(0), __cl(cl)
{
}
