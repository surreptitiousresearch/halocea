#include "dsVECTOR.h"

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::Length @ 0x82697FA0
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<strmVID_BUFFER *, 8>::Length() const
{
    return this->nElem;
}
