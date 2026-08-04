#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "ds_boundary.h"
#include "../ssl/sslOBJ_REF.h"

// dsDATA::StoreValue<sslOBJ_REF> @ 0x825233C8
// Install `val` as the dsDATA's current value. If empty, adopt the sslOBJ_REF type descriptor and
// construct from val. If it already holds an sslOBJ_REF, overwrite in place via CopyObj. Otherwise
// destroy the old value, switch the type descriptor, and construct the new value from val.
template<>
void dsDATA::StoreValue<sslOBJ_REF>(const sslOBJ_REF &val)
{
    const dsDATA_TYPE *destType = dsDATA_TYPE_STORAGE<sslOBJ_REF>::dataType;
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
