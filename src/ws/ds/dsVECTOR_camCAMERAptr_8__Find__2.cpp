#include "dsVECTOR.h"

struct camCAMERA; // boundary — ws-engine cam: render camera (element is a pointer)
struct dsCMP {};   // boundary — pointer-value comparator (empty; comparison is inlined)

// dsVECTOR<camCAMERA*,8>::Find<dsCMP,camCAMERA const*> @ 0x82703CB8
// Linear scan from startPos for the first element pointer equal to `*key` (raw pointer-value
// compare, not a field compare); returns its index, or -1 if none found.
template<>
template<>
int dsVECTOR<camCAMERA *, 8>::Find<dsCMP, camCAMERA const *>(
    camCAMERA const *const &key, const dsCMP & /*cmp*/, int startPos) const
{
    const int count = this->nElem;
    if (startPos >= count)
        return -1;

    camCAMERA *const *elems = this->pData;
    const camCAMERA *target = key;
    int foundIndex = startPos;
    for (int scanIndex = startPos; ; ++scanIndex) {
        const camCAMERA *elem = elems[scanIndex];
        if (elem >= target && target >= elem) // equality via two pointer compares
            break;
        if (++foundIndex == count)
            return -1;
    }
    return foundIndex;
}
