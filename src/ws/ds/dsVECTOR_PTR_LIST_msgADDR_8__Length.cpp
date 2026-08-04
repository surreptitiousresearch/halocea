#include "dsVECTOR.h"
#include "PTR_LIST.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem)

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::Length @ 0x82555FF8
// Number of live elements.
template<>
int dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::Length() const
{
    return this->nElem;
}
