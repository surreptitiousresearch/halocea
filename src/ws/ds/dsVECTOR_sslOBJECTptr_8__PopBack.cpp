#include "dsVECTOR.h"

struct sslOBJECT; // boundary — ssl scripting object; element is a pointer.

// dsVECTOR<sslOBJECT*,8>::PopBack @ 0x82ADA3A0
// Drop the last live element (no-op if already empty) via ShrinkResize.
template<>
void dsVECTOR<sslOBJECT *, 8>::PopBack()
{
    int nElem = this->nElem;
    if (nElem)
        this->ShrinkResize(nElem - 1);
}
