#include "dsVECTOR.h"
#include "../ssl/sslCLASS_ELEMENT.h"

// dsVECTOR<sslCLASS_ELEMENT,8>::Length @ 0x825301C0
// Number of live elements.
template<>
int dsVECTOR<sslCLASS_ELEMENT, 8>::Length() const
{
    return this->nElem;
}
