#include "dsVECTOR.h"
#include "dsCMP.h"

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::Find<dsCMP,strmVID_BUFFER*> @ 0x826983A0
// Linear scan from startPos for the first element pointer equal to `key` (dsCMP compares raw
// pointer identity here — the passed comparator is stateless/unused in the compiled code).
template<>
template<>
int dsVECTOR<strmVID_BUFFER *, 8>::Find<dsCMP, strmVID_BUFFER *>(strmVID_BUFFER *const &key, const dsCMP &, int startPos) const
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
