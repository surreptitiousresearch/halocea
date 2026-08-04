#include "dsVECTOR.h"

class strmVID_MEM_BLOCK; // boundary — strm subsystem video-memory block (element is a pointer)

// dsVECTOR<strmVID_MEM_BLOCK*,8>::Length @ 0x82697FB8
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<strmVID_MEM_BLOCK *, 8>::Length() const
{
    return this->nElem;
}
