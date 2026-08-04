#include "dsVECTOR.h"
#include "dsPARAM.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPARAM,8>::~dsVECTOR @ 0x823D1C58
// Destroy each live parameter's type-erased value via its runtime type descriptor, then free the
// backing storage. Note: dsDATA now has a declared ~dsDATA(), but this body inlines the same
// check-and-Destroy directly on data.type rather than calling it (matches the disassembly).
template<>
dsVECTOR<dsPARAM, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsPARAM *param = &this->pData[index];
        if (param->data.type)
            param->data.type->Destroy(&param->data.storage);
    }
    dlFree(this->pData);
}
