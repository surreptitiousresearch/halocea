#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_boundary.h"
#include "ds_assert_boundary.h"

// dsDATA::SetValue<double> @ 0x823D23B0
// Set this value to `*val`, preserving THIS's existing type on a mismatch. Empty: adopt the double
// type and construct from `val`. Already double: overwrite in place. Otherwise: construct a fresh
// double holding `val`, convert it into THIS's existing type; on success destroy the old value and
// adopt the converted result; on failure keep the old value (and strong-assert).
template<>
void dsDATA::SetValue<double>(const double *val)
{
    const dsDATA_TYPE *storedType = this->type;
    const dsDATA_TYPE *doubleType = dsDATA_TYPE_STORAGE<double>::dataType;

    if (!storedType) {
        doubleType->ConstructFromObj(&this->storage, val);
        this->type = doubleType;
        return;
    }

    if (storedType == doubleType) {
        storedType->CopyObj(val, &this->storage); // overwrite in place
        return;
    }

    // Type mismatch: build a double source value, convert it into THIS's existing type.
    unsigned int srcStorage;       // the temporary double value (8 bytes span two words)
    unsigned int convStorage[11];  // the converted value in THIS's type
    doubleType->Construct(&srcStorage);
    *(double *)doubleType->GetPtr(&srcStorage) = *val; // full 8-byte double copy
    this->type->Construct(convStorage);

    if (doubleType->Convert(&srcStorage, convStorage, this->type, DSD_CONV_STORE)) {
        this->type->Destroy(&this->storage); // drop the old value (storage@0 == this)
        this->storage = convStorage[0];      // adopt the converted result
        doubleType->Destroy(&srcStorage);
    } else {
        doubleType->Destroy(&srcStorage);
        this->type->Destroy(convStorage);
        if (!IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash(
                "0",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_data_impl.hpp",
                138,
                dsStrongAssertMessage);
    }
}
