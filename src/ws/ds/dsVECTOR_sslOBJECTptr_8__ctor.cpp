#include "dsVECTOR.h"

struct sslOBJECT; // boundary — ssl scripting object; element is a pointer.

// dsVECTOR<sslOBJECT*,8>::dsVECTOR @ 0x82AD7B18
// CAVEAT: decompile flagged "local variable allocation has failed"; reconstructed from the
// generic dsVECTOR default constructor idiom confirmed at every other instantiation in this
// corpus — null storage, zero count/capacity, and copy the call-site cookie.
template<>
dsVECTOR<sslOBJECT *, 8>::dsVECTOR(const apCL &cl)
    : pData(0), nElem(0), allocated(0), __cl(cl)
{
}
