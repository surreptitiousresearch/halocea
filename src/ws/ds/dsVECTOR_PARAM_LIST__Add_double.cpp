#include "dsVECTOR_PARAM_LIST.h"

// dsVECTOR_PARAM_LIST::Add<double> @ 0x823E3330
// Append a double-valued parameter named `id`, then refresh the dsPARAM_LIST base view.
// (Value arrives in an FP register; the decompiler's `long double` is a plain double here.)
template<>
void dsVECTOR_PARAM_LIST::Add<double>(dsSTRID id, const double &val)
{
    dsPARAM newParam;             // default: id = dsSTRID(), data.type = null
    newParam.data.type = nullptr;
    dsPARAM *slot = &this->storage.PushBack(newParam);
    if (newParam.data.type)       // temp carried no boxed value; destroy path is a no-op here
        newParam.data.type->Destroy((unsigned int *)&newParam.data);
    slot->id = id;
    slot->data.SetValue<double>(&val);
    this->length = this->storage.nElem;
    this->list = &this->storage[0];
}
