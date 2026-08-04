#include "dsAFFIX_BUILDER.h"

// dsAFFIX_BUILDER::FindAffix @ 0x82A76DD8
// Linear scan for the affix whose name equals `name`; returns its id, or -1 if none. Names match
// on a shared buffer pointer (fast path), or on equal length and equal bytes.
int dsAFFIX_BUILDER::FindAffix(const dsTSTRING<char> &name) const
{
    int index = 0;
    if (this->affixes.nElem <= 0)
        return -1;

    while (true) {
        const AFFIX &affix = this->affixes[index];
        dsTSTRING_BUF_HEADER<char> *wantBuf = name.pBuffer;
        dsTSTRING_BUF_HEADER<char> *affixBuf = affix.name.pBuffer;

        bool isMatch;
        if (affixBuf == wantBuf) {
            isMatch = true;
        } else if (affixBuf->strLen != wantBuf->strLen) {
            isMatch = false;
        } else if (affixBuf->strLen == 0) {
            isMatch = true;
        } else {
            const char *wantChars = wantBuf->str;
            const char *affixChars = affixBuf->str;
            const char *affixEnd = &affixBuf->str[affixBuf->strLen];
            int diff;
            do {
                int wantChar = (unsigned char)*wantChars;
                diff = (unsigned char)*affixChars - wantChar;
                if ((unsigned char)*affixChars != wantChar)
                    break;
                ++affixChars;
                ++wantChars;
            } while (affixChars != affixEnd);
            isMatch = (diff == 0);
        }

        if (isMatch)
            break;
        if (++index >= this->affixes.nElem)
            return -1;
    }
    return index;
}
