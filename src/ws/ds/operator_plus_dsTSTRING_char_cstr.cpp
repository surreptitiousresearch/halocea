#include "dsTSTRING.h"

extern const char dsEmptyCStr[]; // boundary — the shared "" literal at 0x8200155A (empty_string)

// operator+<char>(const dsTSTRING<char>&, const char*) @ 0x823B5518
// Concatenation with a C-string right operand: build a fresh string (returned by value / sret)
// that starts by sharing `a`'s buffer (ref-bump), then appends the NUL-terminated `b` at its end
// via the public Insert(pos, const char*, len=-1) overload. A null `b` becomes the empty string.
template<>
dsTSTRING<char> operator+<char>(const dsTSTRING<char> &a, const char *b)
{
    if (!b)
        b = dsEmptyCStr;
    dsTSTRING<char> result;
    dsTSTRING_BUF_HEADER<char> *aBuffer = a.pBuffer;
    result.pBuffer = a.pBuffer;
    ++aBuffer->refCount;
    result.Insert(result.pBuffer->strLen, b, -1);
    return result;
}
