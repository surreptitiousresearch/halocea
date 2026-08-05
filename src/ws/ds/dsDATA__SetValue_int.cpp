#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_boundary.h"
#include "ds_assert_boundary.h"

// dsDATA::SetValue<int> @ 0x823CFE20
// Set this value to `*val`, preserving THIS's existing type on a mismatch. Empty: adopt the int
// type and construct from `val`. Already int: overwrite in place. Otherwise: construct a fresh int
// holding `val`, convert it into THIS's existing type; on success destroy the old value and adopt
// the converted result; on failure keep the old value (and strong-assert).
template<>
void dsDATA::SetValue<int>(const int *val)
{
    const dsDATA_TYPE *storedType = this->type;
    const dsDATA_TYPE *intType = dsDATA_TYPE_STORAGE<int>::dataType;

    if (!storedType) {
        intType->ConstructFromObj(&this->storage, val);
        this->type = intType;
        return;
    }

    if (storedType == intType) {
        storedType->CopyObj(val, &this->storage); // overwrite in place
        return;
    }

    // Type mismatch: build an int source value, convert it into THIS's existing type.
    unsigned int srcStorage;       // the temporary int value
    unsigned int convStorage[11];  // the converted value in THIS's type
    intType->Construct(&srcStorage);
    *(int *)intType->GetPtr(&srcStorage) = *val;
    this->type->Construct(convStorage);

    if (intType->Convert(&srcStorage, convStorage, this->type, DSD_CONV_STORE)) {
        this->type->Destroy(&this->storage); // drop the old value
        this->storage = convStorage[0];      // adopt the converted result
        intType->Destroy(&srcStorage);
    } else {
        intType->Destroy(&srcStorage);
        this->type->Destroy(convStorage);
        if (!IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash(
                "0",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_data_impl.hpp",
                138,
                empty_string);
    }
}
