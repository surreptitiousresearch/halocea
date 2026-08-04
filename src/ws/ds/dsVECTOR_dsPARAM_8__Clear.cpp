#include "dsVECTOR.h"
#include "dsPARAM.h"

// dsVECTOR<dsPARAM,8>::Clear @ 0x82614920
// Destroy every live parameter's type-erased value via its runtime type descriptor, then
// reset the count. The virtual dispatched is Destroy (DB dsDATA_TYPE_vtbl slot 0xC), passed a
// pointer to the parameter's data storage word.
template<>
void dsVECTOR<dsPARAM, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsPARAM *param = &this->pData[index];
        if (param->data.type)
            param->data.type->Destroy(&param->data.storage);
    }
    this->nElem = 0;
}
