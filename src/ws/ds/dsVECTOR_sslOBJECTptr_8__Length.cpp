#include "dsVECTOR.h"

struct sslOBJECT; // boundary — ssl scripting object; element is a pointer.

// dsVECTOR<sslOBJECT*,8>::Length @ 0x82AD7AE0
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<sslOBJECT *, 8>::Length() const
{
    return this->nElem;
}
