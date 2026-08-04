#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::PopBack @ 0x8261CB48
// Drop the last live element (no-op if already empty). Delegates to ShrinkResize, which dispatches
// the popped element's own type Destroy hook rather than just decrementing nElem.
template<>
void dsVECTOR<dsDATA, 8>::PopBack()
{
    int nElem = this->nElem;
    if (nElem)
        this->ShrinkResize(nElem - 1);
}
