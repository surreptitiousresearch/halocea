#include "dsDATA.h"
#include "dsDATA_TYPE.h"

// dsDATA::StoreValue(const dsDATA&) @ 0x823B3FA0
// Overwrite this dsDATA's value+type by copying from another dsDATA `src` (the non-template copy
// overload, distinct from the templated StoreValue<T>):
//   - src empty            -> drop this value (Destroy) and clear the type.
//   - this empty           -> adopt src's type and copy-construct the value from it.
//   - same type            -> copy-assign the value in place from src (Copy).
//   - different type       -> destroy the old value, adopt src's type, copy-construct from src.
void dsDATA::StoreValue(const dsDATA &src)
{
    const dsDATA_TYPE *srcType = src.type;
    const dsDATA_TYPE *storedType = this->type;
    if (!srcType) {
        if (storedType) {
            storedType->Destroy(&this->storage);
            this->type = nullptr;
        }
        return;
    }
    if (!storedType) {
        this->type = srcType;
        srcType->CopyConstruct(&this->storage, &src.storage);
        return;
    }
    if (storedType == srcType) {
        // Same type fast path: copy-assign in place (vtable slot 0x10, Copy(src, dest)).
        storedType->Copy(&src.storage, &this->storage);
    } else {
        storedType->Destroy(&this->storage);
        // The disasm re-reads src.type after Destroy and re-checks it for null before
        // copy-constructing; src.type is unchanged (== srcType, non-null) so the check is dead.
        this->type = srcType;
        srcType->CopyConstruct(&this->storage, &src.storage);
    }
}
