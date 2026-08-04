#include "dsAFFIX_READER.h"

extern "C" double atof(const char *nptr); // boundary — CRT

// dsAFFIX_READER::GetFloat @ 0x82A755E0
// Look up the named parameter and parse its sub-string as a float. Returns false when the
// parameter is absent.
// Deviation: the decompiler carried atof's double result through fp2 and stored it via a
// double reinterpret; the disasm is `frsp; stfs`, i.e. a plain (float) narrowing store.
bool dsAFFIX_READER::GetFloat(const char *paramName, float *val)
{
    int paramIndex = this->GetParam(paramName);
    if (paramIndex < 0)
        return false;
    *val = (float)atof(this->affSubStr[paramIndex].ptr);
    return true;
}
