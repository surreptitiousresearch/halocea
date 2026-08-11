#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
// @0x8251DBE8 — value under `key` coerced to bool into `*val`; returns whether it existed.

extern int psDataToBool(const dsDATA *obj, int *val);

int psSECTION_INTERFACE::GetBool(const char *key, int *val, unsigned int flags) const
{
    dsDATA value = GetValue(key, flags);
    int found = psDataToBool(&value, val);
    if (value.type)
        value.type->Destroy(&value.storage);
    return found;
}
