#include "psSECTION_INTERFACE.h"
#include "psARRAY.h"
#include "../ds/dsDATA.h"
// 0x8251ED48 — the array value stored under `key`: fetch the raw dsDATA value then coerce it into a
// psARRAY (empty when the key is absent or not array-typed). sret return (swapped with `this`).

// ?psDataToArray@@YAHABVdsDATA@@PAVpsARRAY@@@Z — coerce a dsDATA into a psARRAY. boundary.
extern int psDataToArray(const dsDATA &data, psARRAY *out);

psARRAY psSECTION_INTERFACE::GetArray(const char *key, unsigned int flags) const
{
    psARRAY result;                                // default-constructed (null handle)
    dsDATA value = this->GetValue(key, flags);     // temp; destroyed at scope end
    psDataToArray(value, &result);
    return result;
}
