#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_boundary.h"
#include "../ssl/sslOBJ_REF.h"

// dsDATA::GetValue<sslOBJ_REF> @ 0x82524580
// Retrieve the stored value as an sslOBJ_REF. If the dsDATA holds nothing, fail (0). If the stored
// type already is sslOBJ_REF, copy-assign it out of the type's storage pointer (1). Otherwise
// dispatch the runtime converter from the stored type into the caller's sslOBJ_REF, returning
// whether a converter existed. sslOBJ_REF has a non-trivial copy, so the fast path uses operator=
// rather than a raw scalar load.
template<>
int dsDATA::GetValue<sslOBJ_REF>(sslOBJ_REF &val, dsCONVERTION_TYPE convType) const
{
    const dsDATA_TYPE *storedType = this->type;
    if (!storedType)
        return 0;

    const dsDATA_TYPE *destType = dsDATA_TYPE_STORAGE<sslOBJ_REF>::dataType;
    if (storedType == destType) {
        val = *(const sslOBJ_REF *)storedType->GetPtr((unsigned int *)&this->storage);
        return 1;
    }

    // vtable Convert slot (decompiler named it RawConvert; same slot the bool/float paths call).
    return storedType->Convert(
        (const unsigned int *)storedType->GetPtr((unsigned int *)&this->storage), // src value
        (unsigned int *)&val,                                                     // dest sslOBJ_REF
        destType,                                                                 // dest type
        convType);
}
