#include "dsVECTOR.h"

struct apCOUNTER; // boundary — ap performance counter; element is a pointer.

// dsVECTOR<apCOUNTER*,8>::PopBack @ 0x82715268
// Drop the last live element (no-op if already empty) via ShrinkResize.
template<>
void dsVECTOR<apCOUNTER *, 8>::PopBack()
{
    int nElem = this->nElem;
    if (nElem)
        this->ShrinkResize(nElem - 1);
}
