#include "dsVECTOR.h"

struct sslFOREACH_DATA; // boundary — ssl foreach-loop cursor record; element is a value.

// dsVECTOR<sslFOREACH_DATA,8>::PopBack @ 0x82AF1668
// Drop the last live element (no-op on an empty vector).
template<>
void dsVECTOR<sslFOREACH_DATA, 8>::PopBack()
{
    int nElem = this->nElem;
    if (nElem)
        this->ShrinkResize(nElem - 1);
}
