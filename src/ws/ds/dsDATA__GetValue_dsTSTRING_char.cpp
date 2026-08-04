#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsTSTRING.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_boundary.h"

// dsDATA::GetValue<dsTSTRING<char>> @ 0x823C4A38
// Retrieve the stored value as a string. If the dsDATA holds nothing, fail (0). If the stored type
// is already dsTSTRING<char>, copy-assign it out directly (1). Otherwise dispatch the runtime
// converter from the stored type into the string, returning whether a converter existed.
template<>
int dsDATA::GetValue<dsTSTRING<char>>(dsTSTRING<char> &val, dsCONVERTION_TYPE convType) const
{
    const dsDATA_TYPE *storedType = this->type;
    if (!storedType)
        return 0;

    if (storedType == dsDATA_TYPE_STORAGE<dsTSTRING<char>>::dataType) {
        const dsTSTRING<char> *stringValue =
            (const dsTSTRING<char> *)storedType->GetPtr((unsigned int *)&this->storage);
        val = *stringValue;
        return 1;
    }

    return this->type->Convert(
        (const unsigned int *)storedType->GetPtr((unsigned int *)&this->storage), // src value
        (unsigned int *)&val,                                                     // dest string
        dsDATA_TYPE_STORAGE<dsTSTRING<char>>::dataType,                            // dest type
        convType);
}
