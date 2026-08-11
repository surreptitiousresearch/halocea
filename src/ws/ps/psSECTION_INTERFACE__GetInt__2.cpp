#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
// @0x8251EAD0 — value under `key` coerced to int, or `def` when absent/uncoercible.

extern int psDataToInt(const dsDATA *obj, int *val);

int psSECTION_INTERFACE::GetInt(const char *key, int def, unsigned int flags) const
{
    int result = def;
    dsDATA value = GetValue(key, flags);
    psDataToInt(&value, &result);
    if (value.type)
        value.type->Destroy(&value.storage);
    return result;
}
