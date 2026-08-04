#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_boundary.h"
#include "../ps/psARRAY.h"

// dsDATA::GetValue<psARRAY> @ 0x827AEF30
// Retrieve the stored value as a psARRAY. If the dsDATA holds nothing, fail (0). If the stored
// type already is psARRAY, copy-assign it out of the type's storage pointer (1). Otherwise
// dispatch the runtime converter from the stored type into the caller's psARRAY, returning
// whether a converter existed. Like the psSECTION specialization, psARRAY has a non-trivial
// copy, so the fast path uses operator= rather than a raw scalar load.
template<>
int dsDATA::GetValue<psARRAY>(psARRAY &val, dsCONVERTION_TYPE convType) const
{
    const dsDATA_TYPE *storedType = this->type;
    if (!storedType)
        return 0;

    const dsDATA_TYPE *destType = dsDATA_TYPE_STORAGE<psARRAY>::dataType;
    if (storedType == destType) {
        val = *(const psARRAY *)storedType->GetPtr((unsigned int *)&this->storage);
        return 1;
    }

    // vtable Convert slot (decompiler named it RawConvert; same slot the psSECTION path calls).
    return storedType->Convert(
        (const unsigned int *)storedType->GetPtr((unsigned int *)&this->storage), // src value
        (unsigned int *)&val,                                                     // dest psARRAY
        destType,                                                                 // dest type
        convType);
}
