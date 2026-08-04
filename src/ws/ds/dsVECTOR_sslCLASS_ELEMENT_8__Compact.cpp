#include "dsVECTOR.h"
#include "../ssl/sslCLASS_ELEMENT.h"

// dsVECTOR<sslCLASS_ELEMENT,8>::Compact @ 0x82A3B740
// Shrink the backing storage to exactly fit the live element count.
template<>
void dsVECTOR<sslCLASS_ELEMENT, 8>::Compact()
{
    this->Realloc(this->nElem);
}
