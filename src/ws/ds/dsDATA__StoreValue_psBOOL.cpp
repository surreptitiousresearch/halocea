#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "ds_boundary.h"
#include "../ps/psBOOL.h"

// dsDATA::StoreValue<psBOOL> @ 0x82623BD8 (?StoreValue@dsDATA@@QAAXABVpsBOOL@@@Z)
// Same shape as StoreValue<bool>/StoreValue<int>: install `val` as the dsDATA's current value —
// adopt the psBOOL type descriptor if empty, CopyObj in place if already holding a psBOOL,
// otherwise destroy the old value and construct fresh.
template<>
void dsDATA::StoreValue<psBOOL>(const psBOOL &val)
{
    const dsDATA_TYPE *destType = dsDATA_TYPE_STORAGE<psBOOL>::dataType;
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
