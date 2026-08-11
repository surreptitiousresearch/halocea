#include "psSECTION_INTERFACE.h"
#include "psSECTION.h"
#include "../ds/dsDATA.h"
// @0x8251E018 — look up `key`, coerce the value to a section handle into `*val`, and return whether
// it existed. The value dsDATA is a scratch temp (the decompiler's if(v9) tail is its destructor).

// ?psDataToSection@@YAHABVdsDATA@@PAVpsSECTION@@@Z — coerce a dsDATA into a psSECTION. boundary.
extern int psDataToSection(const dsDATA &data, psSECTION *out);

int psSECTION_INTERFACE::GetSection(const char *key, psSECTION *val, unsigned int flags) const
{
    dsDATA value = this->GetValue(key, flags); // temp; destroyed at scope end
    return psDataToSection(value, val);
}
