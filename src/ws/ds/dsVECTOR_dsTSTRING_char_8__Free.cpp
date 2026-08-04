#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::Free @ 0x832E19F8
// Fully release: Realloc(0) destroys every live element (dropping its buffer reference) via
// ShrinkResize, then frees the (now empty) backing store, zeroing allocated.
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Free()
{
    this->Realloc(0);
}
