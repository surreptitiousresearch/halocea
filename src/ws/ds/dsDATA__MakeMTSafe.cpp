#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_assert_boundary.h"

// dsDATA::MakeMTSafe @ 0x82612C70
// Ensure the stored value is safe to share across threads. An empty value or one whose type reports
// itself MT-safe needs no work (returns 1). Otherwise the stored type names an MT-safe analog type;
// convert this value in place to that type and return whether the conversion succeeded.
int dsDATA::MakeMTSafe()
{
    const dsDATA_TYPE *storedType = this->type;
    if (!storedType)
        return 1;
    if (storedType->IsMTSafe())
        return 1;

    storedType = this->type;
    if (!storedType)
        return 1;

    const dsDATA_TYPE *mtSafeType = storedType->MTSafeAnalog();
    if (!mtSafeType) {
        if (!IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash(
                "_type",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_data_impl.hpp",
                254,
                dsStrongAssertMessage);
        return 0;
    }
    return this->Convert(mtSafeType, DSD_CONV_EXPLICIT);
}
