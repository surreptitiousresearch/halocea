#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
// @0x8251DB78 — value under `key` coerced to int into `*val`; returns whether it existed.

extern int psDataToInt(const dsDATA *obj, int *val);

int psSECTION_INTERFACE::GetInt(const char *key, int *val, unsigned int flags) const
{
    dsDATA value = GetValue(key, flags);
    int found = psDataToInt(&value, val);
    if (value.type)
        value.type->Destroy(&value.storage);
    return found;
}
