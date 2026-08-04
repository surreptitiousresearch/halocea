#include "dsVECTOR.h"
#include "../msg/saLIST.h"

// dsVECTOR<saLIST,8>::Length @ 0x82AC58D0
// Number of live elements.
template<>
int dsVECTOR<saLIST, 8>::Length() const
{
    return this->nElem;
}
