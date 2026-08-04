#include "dsSTATIC_PARAM_LIST.h"

// dsSTATIC_PARAM_LIST<2>::dsSTATIC_PARAM_LIST() @ 0x823D0788
// Default-construct the two inline parameter slots (empty id + null-type value), then publish the
// dsPARAM_LIST base view (list = &storage[0], length = 0).
template<>
dsSTATIC_PARAM_LIST<2>::dsSTATIC_PARAM_LIST()
{
    dsPARAM *first = this->storage;
    this->length = 0;
    this->list = nullptr;

    for (int i = 0; i < 2; ++i) {
        this->storage[i].id = dsSTRID();        // default: empty interned id
        this->storage[i].data.type = nullptr;
    }

    this->list = first;
    this->length = 0;
}
