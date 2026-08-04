#include "dsAFFIX_READER.h"

// dsAFFIX_READER::FindAffix @ 0x82A75038
// Scan the recorded sub-strings for the first still-unclaimed one (type == 0) that matches
// `affix`. Unless `asSubString` is set, the recorded length must equal the affix length exactly;
// the bytes are then compared. Returns the sub-string index, or -1 if none match.
int dsAFFIX_READER::FindAffix(const char *affix, int asSubString) const
{
    const char *scan = affix;
    while (*scan++)
        ;
    int affixLen = (int)(scan - affix - 1);

    int nSubStr = this->nSubStr;
    if (nSubStr <= 0)
        return -1;

    for (int index = 0; ; ++index) {
        const AFF_SUBSTR *substr = &this->affSubStr[index];
        if (substr->type == 0 && (asSubString || substr->length == affixLen)) {
            const unsigned char *candidate = (const unsigned char *)substr->ptr;
            const char *want = affix;
            int diff = 0;
            if (affixLen) {
                do {
                    diff = (unsigned char)*want - *candidate;
                    if ((unsigned char)*want != *candidate)
                        break;
                    ++want;
                    ++candidate;
                } while (want != &affix[affixLen]);
            }
            if (!diff)
                return index;
        }
        if (index + 1 >= nSubStr)
            return -1;
    }
}
