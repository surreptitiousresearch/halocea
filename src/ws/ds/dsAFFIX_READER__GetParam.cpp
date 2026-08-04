#include "dsAFFIX_READER.h"

// dsAFFIX_READER::GetParam @ 0x82A750F8
// For the affix currently matched into curSubStr, walk its trailing parameter sub-strings (which
// come as name/value pairs) and return the index of the VALUE sub-string whose preceding type-1
// NAME sub-string equals `paramName` (same length and bytes). Returns -1 if none.
int dsAFFIX_READER::GetParam(const char *paramName) const
{
    int curSubStr = this->curSubStr;
    if (curSubStr < 0)
        return -1;

    const char *scan = paramName;
    while (*scan++)
        ;
    int paramNameLen = (int)(scan - paramName - 1);

    int nSubStr = this->nSubStr;
    int index = curSubStr + 1;
    if (index < nSubStr) {
        // affSubStr entries alternate NAME (type 1) / VALUE; step two per parameter.
        for (const AFF_SUBSTR *sub = &this->affSubStr[index]; sub->type == 1; sub += 2) {
            if (sub->length == paramNameLen) {
                const unsigned char *subChars = (const unsigned char *)sub->ptr;
                const char *want = paramName;
                int diff = 0;
                if (paramNameLen) {
                    const unsigned char *subEnd = subChars + paramNameLen;
                    do {
                        int wantChar = (unsigned char)*want;
                        diff = *subChars - wantChar;
                        if (*subChars != wantChar)
                            break;
                        ++subChars;
                        ++want;
                    } while (subChars != subEnd);
                }
                if (!diff)
                    return index + 1;
            }
            index += 2;
            if (index >= nSubStr)
                return -1;
        }
    }
    return -1;
}
