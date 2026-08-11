#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
// @0x8251EB30 — value under `key` coerced to bool, or `def` when absent/uncoercible.

extern int psDataToBool(const dsDATA *obj, int *val);

int psSECTION_INTERFACE::GetBool(const char *key, int def, unsigned int flags) const
{
    int result = def;
    dsDATA value = GetValue(key, flags);
    psDataToBool(&value, &result);
    if (value.type)
        value.type->Destroy(&value.storage);
    return result;
}
