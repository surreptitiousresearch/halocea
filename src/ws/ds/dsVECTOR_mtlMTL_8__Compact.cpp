#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"

// dsVECTOR<mtlMTL,8>::Compact @ 0x82688298
// Shrink the backing storage to exactly fit the live elements.
template<>
void dsVECTOR<mtlMTL, 8>::Compact()
{
    this->Realloc(this->nElem);
}
