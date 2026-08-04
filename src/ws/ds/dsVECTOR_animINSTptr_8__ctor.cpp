#include "dsVECTOR.h"

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::dsVECTOR @ 0x823ED580
// CAVEAT: decompile flagged "local variable allocation has failed"; reconstructed from the
// generic dsVECTOR default constructor idiom confirmed at every other instantiation in this
// corpus — null storage, zero count/capacity, and copy the call-site cookie.
// Default-construct an empty vector using the caller-supplied allocation call-site cookie.
template<>
dsVECTOR<animINST *, 8>::dsVECTOR(const apCL &cl)
    : pData(0), nElem(0), allocated(0), __cl(cl)
{
}
