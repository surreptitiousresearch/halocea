#include "dsAFFIX_READER.h"

extern "C" void *memcpy(void *dst, const void *src, unsigned int n); // boundary — CRT

// dsAFFIX_READER::GetString @ 0x82A75DB0
// Look up the named parameter; if present, copy its recorded sub-string (ptr/length) into the
// caller-supplied string and return true. Returns false when the parameter is absent.
bool dsAFFIX_READER::GetString(const char *paramName, dsTSTRING<char> *val)
{
    int paramIndex = this->GetParam(paramName);
    if (paramIndex < 0)
        return false;

    // Decompiler folds the affSubStr base offset into the field indices; this is affSubStr[paramIndex].
    AFF_SUBSTR *sub = &this->affSubStr[paramIndex];
    char *dst = val->Lock(sub->length);
    memcpy(dst, sub->ptr, sub->length);
    val->Unlock();
    return true;
}
