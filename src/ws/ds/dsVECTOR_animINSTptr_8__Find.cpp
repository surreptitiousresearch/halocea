#include "dsVECTOR.h"
#include "dsCMP.h"

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::Find<dsCMP,animINST*> @ 0x82879C78
// Linear scan from startPos for the first element pointer equal to `key` (dsCMP compares raw
// pointer identity here — the passed comparator is stateless/unused in the compiled code).
template<>
template<>
int dsVECTOR<animINST *, 8>::Find<dsCMP, animINST *>(animINST *const &key, const dsCMP &, int startPos) const
{
    if (startPos >= this->nElem)
        return -1;
    for (int index = startPos; index != this->nElem; ++index) {
        if (this->pData[index] == key)
            return index;
    }
    return -1;
}
