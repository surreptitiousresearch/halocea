#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "ds_boundary.h"

// dsDATA::StoreValue<bool> @ 0x823D2078
// Install `val` as the dsDATA's current value. If empty, adopt the bool type descriptor and
// construct from val. If it already holds bool, overwrite in place via CopyObj. Otherwise destroy
// the old value, switch the type descriptor to bool, and construct the new value from val.
template<>
void dsDATA::StoreValue<bool>(const bool &val)
{
    const dsDATA_TYPE *destType = dsDATA_TYPE_STORAGE<bool>::dataType;
    const dsDATA_TYPE *storedType = this->type;
    if (!storedType) {
        this->type = destType;
        if (destType)
            destType->ConstructFromObj(&this->storage, &val);
        return;
    }
    if (storedType == destType) {
        storedType->CopyObj(&val, &this->storage);
    } else {
        storedType->Destroy(&this->storage);
        this->type = destType;
        destType->ConstructFromObj(&this->storage, &val);
    }
}
