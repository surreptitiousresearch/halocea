#include "dsVECTOR_PARAM_LIST.h"
#include "dsTSTRING.h"

// dsVECTOR_PARAM_LIST::Set<dsTSTRING<char>> @ 0x823E31A8
// Upsert a string-valued parameter named `id`: overwrite the value of the existing entry with that
// name if present, otherwise append a fresh entry and refresh the dsPARAM_LIST base view.
template<>
void dsVECTOR_PARAM_LIST::Set<dsTSTRING<char>>(dsSTRID id, const dsTSTRING<char> &val)
{
    dsPARAM *match = nullptr;
    if (this->length > 0) {
        dsPARAM *list = this->list;
        for (int idx = 0; idx < this->length; ++idx) {
            if (id.id == list[idx].id.id) {
                match = &list[idx];
                break;
            }
        }
    }

    if (match) {
        match->id = id;
        match->data.SetValue<dsTSTRING<char>>(&val);
        return;
    }

    dsPARAM newParam;             // default: id = dsSTRID(), data.type = null
    newParam.data.type = nullptr;
    dsPARAM *slot = &this->storage.PushBack(newParam);
    if (newParam.data.type)       // temp carried no boxed value; destroy path is a no-op here
        newParam.data.type->Destroy((unsigned int *)&newParam.data);
    slot->id = id;
    slot->data.SetValue<dsTSTRING<char>>(&val);
    this->length = this->storage.nElem;
    this->list = &this->storage[0];
}
