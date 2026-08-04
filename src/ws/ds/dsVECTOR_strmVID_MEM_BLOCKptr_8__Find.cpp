#include "dsVECTOR.h"
#include "dsCMP.h"

class strmVID_MEM_BLOCK; // boundary — strm subsystem video-memory block (element is a pointer)

// dsVECTOR<strmVID_MEM_BLOCK*,8>::Find<dsCMP,strmVID_MEM_BLOCK*> @ 0x826983F8
// Linear scan from startPos for the first element pointer equal to `key` (dsCMP compares raw
// pointer identity here — the passed comparator is stateless/unused in the compiled code).
template<>
template<>
int dsVECTOR<strmVID_MEM_BLOCK *, 8>::Find<dsCMP, strmVID_MEM_BLOCK *>(strmVID_MEM_BLOCK *const &key, const dsCMP &, int startPos) const
{
    int nElem = this->nElem;
    if (startPos >= nElem)
        return -1;
    for (int index = startPos; index != nElem; ++index) {
        if (this->pData[index] == key)
            return index;
    }
    return -1;
}
