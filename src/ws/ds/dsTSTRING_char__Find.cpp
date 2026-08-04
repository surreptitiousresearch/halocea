#include <ctype.h>
#include "dsTSTRING.h"

// dsTSTRING<char>::Find @ 0x824F7F90
// Naive substring search: for each candidate start position at or after `start`, match the
// NUL-terminated `str` character-by-character against the buffer. Returns the index of the first
// full match, or -1 when `start` is out of range, `str` is null, or no match exists. const (QBA).
template<>
int dsTSTRING<char>::Find(const char *str, int start, int noCase) const
{
    if (start < 0 || start >= this->pBuffer->strLen || !str)
        return -1;

    if (noCase) {
        for (int pos = start; pos < this->pBuffer->strLen; ++pos) {
            for (int matchLen = 0; ; ++matchLen) {
                if (!str[matchLen])
                    return pos; // reached the end of `str`: full match at `pos`
                if (pos + matchLen >= this->pBuffer->strLen)
                    return -1;  // ran off the end of the buffer before matching all of `str`
                if (tolower(this->pBuffer->str[pos + matchLen]) != tolower(str[matchLen]))
                    break;
            }
        }
    } else {
        for (int pos = start; pos < this->pBuffer->strLen; ++pos) {
            for (int matchLen = 0; ; ++matchLen) {
                if (!str[matchLen])
                    return pos;
                if (pos + matchLen >= this->pBuffer->strLen)
                    return -1;
                if (this->pBuffer->str[pos + matchLen] != str[matchLen])
                    break;
            }
        }
    }
    return -1;
}
