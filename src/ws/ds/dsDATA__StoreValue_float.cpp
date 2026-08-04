#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "ds_boundary.h"

// dsDATA::StoreValue<float> @ 0x823EADC0
// Install `val` as the dsDATA's current value. If empty, adopt the float type descriptor and
// construct from val. If it already holds float, overwrite in place via CopyObj. Otherwise destroy
// the old value, switch the type descriptor to float, and construct the new value from val.
// Note: `val` is passed by const reference (a GPR pointer), so there is no float-argument slot to
// skip here — the value itself is never loaded into a FP register by this function.
template<>
void dsDATA::StoreValue<float>(const float &val)
{
    const dsDATA_TYPE *destType = dsDATA_TYPE_STORAGE<float>::dataType;
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
