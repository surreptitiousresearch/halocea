#include "dsVECTOR_PARAM_LIST.h"

// dsVECTOR_PARAM_LIST::Add<int> @ 0x823E32A0
// Append an int-valued parameter named `id`, then refresh the dsPARAM_LIST base view.
template<>
void dsVECTOR_PARAM_LIST::Add<int>(dsSTRID id, const int &val)
{
    dsPARAM newParam;             // default: id = dsSTRID(), data.type = null
    newParam.data.type = nullptr;
    dsPARAM *slot = &this->storage.PushBack(newParam);
    if (newParam.data.type)       // temp carried no boxed value; destroy path is a no-op here
        newParam.data.type->Destroy((unsigned int *)&newParam.data);
    slot->id = id;
    slot->data.SetValue<int>(&val);
    this->length = this->storage.nElem;
    this->list = &this->storage[0];
}
