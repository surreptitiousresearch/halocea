#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_boundary.h"
#include "ds_assert_boundary.h"

// dsDATA::SetValue<float> @ 0x825EE5A8
// Set this value to `*val`, preserving THIS's existing type on a mismatch. Empty: adopt the float
// type and construct from `val`. Already float: overwrite in place. Otherwise: construct a fresh
// float holding `val`, convert it into THIS's existing type; on success destroy the old value and
// adopt the converted result; on failure keep the old value (and strong-assert).
template<>
void dsDATA::SetValue<float>(const float *val)
{
    const dsDATA_TYPE *storedType = this->type;
    const dsDATA_TYPE *floatType = dsDATA_TYPE_STORAGE<float>::dataType;

    if (!storedType) {
        floatType->ConstructFromObj(&this->storage, val);
        this->type = floatType;
        return;
    }

    if (storedType == floatType) {
        storedType->CopyObj(val, &this->storage); // overwrite in place
        return;
    }

    // Type mismatch: build a float source value, convert it into THIS's existing type.
    unsigned int srcStorage;       // the temporary float value
    unsigned int convStorage[4];   // the converted value in THIS's type
    floatType->Construct(&srcStorage);
    *(float *)floatType->GetPtr(&srcStorage) = *val;
    this->type->Construct(&convStorage[0]);

    if (floatType->Convert(&srcStorage, &convStorage[0], this->type, DSD_CONV_STORE)) {
        this->type->Destroy(&this->storage); // drop the old value (storage@0 == this)
        this->storage = convStorage[0];      // adopt the converted result
        floatType->Destroy(&srcStorage);
    } else {
        floatType->Destroy(&srcStorage);
        this->type->Destroy(&convStorage[0]);
        if (!IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash(
                "0",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_data_impl.hpp",
                138,
                empty_string);
    }
}
