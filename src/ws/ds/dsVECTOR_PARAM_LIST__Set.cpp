#include "dsVECTOR_PARAM_LIST.h"

// dsVECTOR_PARAM_LIST::Set @ 0x82618530
// Upsert the parameter named `param.id` from `param`: overwrite the existing entry with that name
// if present, otherwise append a copy and refresh the dsPARAM_LIST base view.
void dsVECTOR_PARAM_LIST::Set(const dsPARAM &param)
{
    const char *id = param.id.id;

    dsPARAM *match = nullptr;
    if (this->length > 0) {
        dsPARAM *list = this->list;
        for (int idx = 0; idx < this->length; ++idx) {
            if (id == list[idx].id.id) {
                match = &this->storage[idx];
                break;
            }
        }
    }

    if (match) {
        match->id.id = param.id.id;
        match->data.SetValue(&param.data);
        return;
    }

    dsPARAM newParam;             // default: id = dsSTRID(), data.type = null
    newParam.data.type = nullptr;
    dsPARAM *slot = &this->storage.PushBack(newParam);
    if (newParam.data.type)       // temp carried no boxed value; destroy path is a no-op here
        newParam.data.type->Destroy((unsigned int *)&newParam.data);
    slot->id.id = param.id.id;
    slot->data.SetValue(&param.data);
    this->length = this->storage.nElem;
    this->list = &this->storage[0];
}
