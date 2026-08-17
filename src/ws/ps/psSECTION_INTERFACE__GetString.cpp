/* ?GetString@psSECTION_INTERFACE@@QBAHPBDPADHK@Z @0x8251DF28 */
#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
// 0x8251DF54 — value under `key` coerced to a NUL-terminated string copied into `buf` (capacity
// `bufLen`); returns whether it existed.

extern int psDataToString(const dsDATA *obj, char *buf, int bufLen); // boundary — not reversed here

int psSECTION_INTERFACE::GetString(const char *key, char *buf, int bufLen, unsigned int flags) const
{
    dsDATA value = GetValue(key, flags);
    int found = psDataToString(&value, buf, bufLen);
    if (value.type)
        value.type->Destroy(&value.storage);
    return found;
}
