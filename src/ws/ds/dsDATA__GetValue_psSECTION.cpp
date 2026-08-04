#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_boundary.h"
#include "../ps/psSECTION.h"

// dsDATA::GetValue<psSECTION> @ 0x8251A850
// Retrieve the stored value as a psSECTION. If the dsDATA holds nothing, fail (0). If the stored
// type already is psSECTION, copy-assign it out of the type's storage pointer (1). Otherwise
// dispatch the runtime converter from the stored type into the caller's psSECTION, returning
// whether a converter existed. Unlike the bool/float specializations, psSECTION has a non-trivial
// copy, so the fast path uses operator= rather than a raw scalar load.
template<>
int dsDATA::GetValue<psSECTION>(psSECTION &val, dsCONVERTION_TYPE convType) const
{
    const dsDATA_TYPE *storedType = this->type;
    if (!storedType)
        return 0;

    const dsDATA_TYPE *destType = dsDATA_TYPE_STORAGE<psSECTION>::dataType;
    if (storedType == destType) {
        val = *(const psSECTION *)storedType->GetPtr((unsigned int *)&this->storage);
        return 1;
    }

    // vtable Convert slot (decompiler named it RawConvert; same slot the bool/float paths call).
    return storedType->Convert(
        (const unsigned int *)storedType->GetPtr((unsigned int *)&this->storage), // src value
        (unsigned int *)&val,                                                     // dest psSECTION
        destType,                                                                 // dest type
        convType);
}
