#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_boundary.h"

// dsDATA::GetValue<int> @ 0x823C3520
// Retrieve the stored value as an int. If the dsDATA holds nothing, fail (0). If the stored type is
// already int, read it directly via GetPtr (1). Otherwise dispatch the runtime converter from the
// stored type into an int, returning whether a converter existed.
template<>
int dsDATA::GetValue<int>(int &val, dsCONVERTION_TYPE convType) const
{
    const dsDATA_TYPE *storedType = this->type;
    if (!storedType)
        return 0;

    if (storedType == dsDATA_TYPE_STORAGE<int>::dataType) {
        val = *(const int *)storedType->GetPtr((unsigned int *)&this->storage);
        return 1;
    }

    return this->type->Convert(
        (const unsigned int *)storedType->GetPtr((unsigned int *)&this->storage), // src value
        (unsigned int *)&val,                                                     // dest int
        dsDATA_TYPE_STORAGE<int>::dataType,                                        // dest type
        convType);
}
