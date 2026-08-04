#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_boundary.h"

// dsDATA::GetValue<bool> @ 0x823B34B8
// Retrieve the stored value as a bool. If the dsDATA holds nothing, fail (0). If the stored type
// is already bool, read it directly via GetPtr (1). Otherwise dispatch the runtime converter from
// the stored type into a bool, returning whether a converter existed.
template<>
int dsDATA::GetValue<bool>(bool &val, dsCONVERTION_TYPE convType) const
{
    const dsDATA_TYPE *storedType = this->type;
    if (!storedType)
        return 0;

    if (storedType == dsDATA_TYPE_STORAGE<bool>::dataType) {
        val = *(const bool *)storedType->GetPtr((unsigned int *)&this->storage);
        return 1;
    }

    return this->type->Convert(
        (const unsigned int *)storedType->GetPtr((unsigned int *)&this->storage), // src value
        (unsigned int *)&val,                                                     // dest bool
        dsDATA_TYPE_STORAGE<bool>::dataType,                                       // dest type
        convType);
}
