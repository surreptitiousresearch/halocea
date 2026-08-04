#include "dsDATA.h"

// dsDATA::Convert @ 0x823C2B30
// Convert this value to `type` in place. If already that type, succeed immediately; if either the
// current or target type is null, fail. Otherwise construct a fresh value of `type` into scratch,
// run the current type's converter into it, and on success replace this value+type with it.
int dsDATA::Convert(const dsDATA_TYPE *type, dsCONVERTION_TYPE convType)
{
    const dsDATA_TYPE *curType = this->type;
    if (curType == type)
        return 1;
    if (!curType || !type)
        return 0;

    unsigned int convertedStorage[12]; // scratch value of the target type
    type->Construct(convertedStorage);
    if (!this->type->Convert(&this->storage, convertedStorage, type, convType)) {
        type->Destroy(convertedStorage);
        return 0;
    }
    this->type->Destroy(&this->storage);
    this->type = type;
    this->storage = convertedStorage[0];
    return 1;
}
