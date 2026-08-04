#include "dsTSTRING.h"
#include <string.h>

// operator+<char>(const dsTSTRING<char>&, const dsTSTRING<char>&) @ 0x823B5448
// Concatenation: build a fresh string (returned by value / sret) that starts by sharing `a`'s
// buffer (ref-bump), opens a gap sized for `b` at its end (Insert's protected grow-and-shift
// overload, which forces a private buffer), then copies `b`'s characters into the gap.
template<>
dsTSTRING<char> operator+<char>(const dsTSTRING<char> &a, const dsTSTRING<char> &b)
{
    dsTSTRING<char> result;
    result.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *aBuffer = a.pBuffer;
    result.pBuffer = a.pBuffer;
    ++aBuffer->refCount;

    int insertAt = result.pBuffer->strLen;
    result.Insert(insertAt, b.pBuffer->strLen); // protected grow-and-shift gap
    memcpy(&result.pBuffer->str[insertAt], b.pBuffer->str, b.pBuffer->strLen);
    return result;
}
