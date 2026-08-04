#include "dsVECTOR.h"

class strmVID_MEM_BLOCK; // boundary — strm subsystem video-memory block (element is a pointer)

// dsVECTOR<strmVID_MEM_BLOCK*,8>::dsVECTOR @ 0x82698010
// CAVEAT: decompile flagged "local variable allocation has failed"; reconstructed from the
// generic dsVECTOR default constructor idiom confirmed at every other instantiation in this
// corpus — null storage, zero count/capacity, and copy the call-site cookie.
template<>
dsVECTOR<strmVID_MEM_BLOCK *, 8>::dsVECTOR(const apCL &cl)
    : pData(0), nElem(0), allocated(0), __cl(cl)
{
}
