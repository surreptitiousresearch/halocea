#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "ds_boundary.h"
#include "NULL_TYPE.h"

// dsDATA::StoreValue<NULL_TYPE> @ 0x82A37D08
// Install `val` as the dsDATA's current value. NULL_TYPE is the empty/void variant: its type
// descriptor's Construct/CopyObj/Destroy are trivial, but the store dispatch is structurally
// identical to every other StoreValue specialization (verified against the disassembly). If empty,
// adopt the NULL_TYPE descriptor and construct from val; if it already holds NULL_TYPE, overwrite
// in place via CopyObj; otherwise destroy the old value, switch the descriptor, and reconstruct.
template<>
void dsDATA::StoreValue<NULL_TYPE>(const NULL_TYPE &val)
{
    const dsDATA_TYPE *destType = dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType;
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
