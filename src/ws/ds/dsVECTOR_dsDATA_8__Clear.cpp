#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::Clear @ 0x8261AFB0
// Destroy every live type-erased value via its runtime type descriptor, then reset the count.
// The virtual dispatched is Destroy (DB dsDATA_TYPE_vtbl slot 0xC), passed a pointer to the
// element's storage word.
template<>
void dsVECTOR<dsDATA, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        const dsDATA_TYPE *type = this->pData[index].type;
        if (type)
            type->Destroy(&this->pData[index].storage);
    }
    this->nElem = 0;
}
