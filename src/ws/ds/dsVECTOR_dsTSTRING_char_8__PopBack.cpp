#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::PopBack @ 0x825BCF68
// Drop the last live element (releasing its buffer reference via ShrinkResize); a no-op when empty.
template<>
void dsVECTOR<dsTSTRING<char>, 8>::PopBack()
{
    if (this->nElem)
        this->ShrinkResize(this->nElem - 1);
}
