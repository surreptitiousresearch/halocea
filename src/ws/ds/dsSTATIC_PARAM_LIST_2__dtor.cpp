#include "dsSTATIC_PARAM_LIST.h"

// dsSTATIC_PARAM_LIST<2>::~dsSTATIC_PARAM_LIST() @ 0x823D0720
// Destroy each of the two inline parameter slots whose value is non-empty, dispatching the stored
// type's Destroy virtual (vtable slot 3). Iterates high slot to low, matching the emitted code.
template<>
dsSTATIC_PARAM_LIST<2>::~dsSTATIC_PARAM_LIST()
{
    for (int i = 1; i >= 0; --i) {
        dsDATA &data = this->storage[i].data;
        if (data.type)
            data.type->Destroy(&data.storage);
    }
}
