#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
#include "../ds/dsTSTRING.h"
// 0x8251DEB8 — value under `key` coerced into the dsTSTRING `*val`; returns whether it existed.
// The value dsDATA is a scratch temp (the decompiler's if(v9) tail is its destructor).

extern int psDataToString(const dsDATA &obj, dsTSTRING<char> *val); // boundary — dsDATA -> dsTSTRING

int psSECTION_INTERFACE::GetDSString(const char *key, dsTSTRING<char> *val, unsigned int flags) const
{
    dsDATA value = this->GetValue(key, flags);
    int found = psDataToString(value, val);
    if (value.type)
        value.type->Destroy(&value.storage);
    return found;
}
