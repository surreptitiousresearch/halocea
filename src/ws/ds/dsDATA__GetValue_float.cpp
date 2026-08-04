#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_boundary.h"

// dsDATA::GetValue<float> @ 0x823F4238
// Retrieve the stored value as a float. If the dsDATA holds nothing, fail (0). If the stored type
// is already float, read it directly via GetPtr (1). Otherwise dispatch the runtime converter from
// the stored type into a float, returning whether a converter existed.
template<>
int dsDATA::GetValue<float>(float &val, dsCONVERTION_TYPE convType) const
{
    const dsDATA_TYPE *storedType = this->type;
    if (!storedType)
        return 0;

    if (storedType == dsDATA_TYPE_STORAGE<float>::dataType) {
        val = *(const float *)storedType->GetPtr((unsigned int *)&this->storage);
        return 1;
    }

    return this->type->Convert(
        (const unsigned int *)storedType->GetPtr((unsigned int *)&this->storage), // src value
        (unsigned int *)&val,                                                     // dest float
        dsDATA_TYPE_STORAGE<float>::dataType,                                      // dest type
        convType);
}
