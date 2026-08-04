#include "dsVECTOR.h"

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::Clear @ 0x82697FE0
// Pointer elements are trivially destructible, so the generic per-element destructor loop is
// empty and this collapses to resetting the live count.
template<>
void dsVECTOR<strmVID_BUFFER *, 8>::Clear()
{
    this->nElem = 0;
}
