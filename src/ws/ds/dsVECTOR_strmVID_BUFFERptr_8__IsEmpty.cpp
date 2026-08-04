#include "dsVECTOR.h"

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::IsEmpty @ 0x82697F90
// True when there are no live elements.
template<>
bool dsVECTOR<strmVID_BUFFER *, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
