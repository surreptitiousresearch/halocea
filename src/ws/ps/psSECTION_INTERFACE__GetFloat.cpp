#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
// 0x8251DC58 — look up `key`, coerce the value to float into `*val`, and return whether it existed.
// The value dsDATA is a scratch temp (the decompiler's if(v9) tail is its scope-end destructor).

// ?psDataToFloat@@YAHABVdsDATA@@PAM@Z — coerce a dsDATA into a float (in/out). boundary.
extern int psDataToFloat(const dsDATA &data, float *value);

int psSECTION_INTERFACE::GetFloat(const char *key, float *val, unsigned int flags) const
{
    dsDATA value = this->GetValue(key, flags); // temp; destroyed at scope end
    return psDataToFloat(value, val);
}
