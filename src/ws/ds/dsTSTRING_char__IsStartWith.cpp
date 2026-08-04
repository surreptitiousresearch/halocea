#include "dsTSTRING.h"

extern "C" int tolower(int c); // boundary — CRT

// dsTSTRING<char>::IsStartWith @ 0x823B0A00
// Return whether the substring beginning at character `ofs` starts with the NUL-terminated `s`,
// comparing case-insensitively when `noCase`. A `s` that runs out of characters (or an `ofs`
// already at/past the end with an empty `s`) counts as a match.
template<>
bool dsTSTRING<char>::IsStartWith(const char *s, bool noCase, int ofs) const
{
    dsTSTRING_BUF_HEADER<char> *buffer = this->pBuffer;
    if (ofs >= buffer->strLen)
        return *(const unsigned char *)s == 0; // nothing left to match against

    int pos = ofs;
    while (true) {
        if (!*s)
            return true; // pattern exhausted — prefix matched
        char stored = buffer->str[pos];
        bool equal;
        if (noCase)
            equal = tolower((unsigned char)*s) == tolower(stored);
        else
            equal = *s == stored;
        if (!equal)
            return false;
        buffer = this->pBuffer;
        ++pos;
        ++s;
        if (pos >= buffer->strLen)
            return *(const unsigned char *)s == 0; // reached end of this string
    }
}
