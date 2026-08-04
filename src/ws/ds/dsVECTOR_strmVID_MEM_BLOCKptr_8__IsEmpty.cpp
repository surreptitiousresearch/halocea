#include "dsVECTOR.h"

class strmVID_MEM_BLOCK; // boundary — strm subsystem video-memory block (element is a pointer)

// dsVECTOR<strmVID_MEM_BLOCK*,8>::IsEmpty @ 0x82697FA8
// True when there are no live elements.
template<>
bool dsVECTOR<strmVID_MEM_BLOCK *, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
