#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsTSTRING.h"
#include "ds_boundary.h"

// dsDATA::StoreValue<dsTSTRING<char>> @ 0x823B4218
// Install `val` as the dsDATA's current value. If empty, adopt the string type descriptor and
// construct from val. If it already holds a dsTSTRING<char>, overwrite in place via CopyObj.
// Otherwise destroy the old value, switch the type descriptor, and construct the new value.
template<>
void dsDATA::StoreValue<dsTSTRING<char> >(const dsTSTRING<char> &val)
{
    const dsDATA_TYPE *destType = dsDATA_TYPE_STORAGE<dsTSTRING<char> >::dataType;
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
