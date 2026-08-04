#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "dsTSTRING.h"
#include "ds_boundary.h"
#include "ds_assert_boundary.h"

// dsDATA::SetValue<dsTSTRING<char>> @ 0x823D44B8
// Set this value to `*val`, preserving THIS's existing type on a mismatch. Empty: adopt the string
// type and construct from `val`. Already a string: overwrite in place. Otherwise: construct a fresh
// string, copy-assign `val` into it, convert it into THIS's existing type; on success destroy the
// old value and adopt the converted result; on failure keep the old value (and strong-assert).
template<>
void dsDATA::SetValue<dsTSTRING<char>>(const dsTSTRING<char> *val)
{
    const dsDATA_TYPE *storedType = this->type;
    const dsDATA_TYPE *stringType = dsDATA_TYPE_STORAGE<dsTSTRING<char>>::dataType;

    if (!storedType) {
        stringType->ConstructFromObj(&this->storage, val);
        this->type = stringType;
        return;
    }

    if (storedType == stringType) {
        storedType->CopyObj(val, &this->storage); // overwrite in place
        return;
    }

    // Type mismatch: build a string source value, convert it into THIS's existing type.
    unsigned int srcStorage;       // the temporary dsTSTRING<char> value
    unsigned int convStorage[11];  // the converted value in THIS's type
    stringType->Construct(&srcStorage);
    *(dsTSTRING<char> *)stringType->GetPtr(&srcStorage) = *val; // dsTSTRING<char>::operator=
    this->type->Construct(&convStorage[0]);

    if (stringType->Convert(&srcStorage, &convStorage[0], this->type, DSD_CONV_STORE)) {
        this->type->Destroy(&this->storage); // drop the old value (storage@0 == this)
        this->storage = convStorage[0];      // adopt the converted result
        stringType->Destroy(&srcStorage);
    } else {
        stringType->Destroy(&srcStorage);
        this->type->Destroy(&convStorage[0]);
        if (!IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash(
                "0",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_data_impl.hpp",
                138,
                dsStrongAssertMessage);
    }
}
