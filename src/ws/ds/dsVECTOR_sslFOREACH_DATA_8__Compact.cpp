#include "dsVECTOR.h"

struct sslFOREACH_DATA; // boundary — ssl foreach-loop cursor record; element is a value.

// dsVECTOR<sslFOREACH_DATA,8>::Compact @ 0x82AF2670
// Shrink the backing storage to exactly fit the live element count.
template<>
void dsVECTOR<sslFOREACH_DATA, 8>::Compact()
{
    this->Realloc(this->nElem);
}
