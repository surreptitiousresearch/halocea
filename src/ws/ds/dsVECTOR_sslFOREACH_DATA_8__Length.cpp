#include "dsVECTOR.h"

struct sslFOREACH_DATA; // boundary — ssl foreach-loop cursor record; element is a value.

// dsVECTOR<sslFOREACH_DATA,8>::Length @ 0x82AF03C8
// Number of live elements.
template<>
int dsVECTOR<sslFOREACH_DATA, 8>::Length() const
{
    return this->nElem;
}
