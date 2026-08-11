// FUNCTION_INDEX entry: operator+<char>(const dsTSTRING<char>&, const char*) @0x823B5518 (??$?HD@@YA?AV?$dsTSTRING@D@@ABV0@PBD@Z)
#include "dsTSTRING.h"

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// operator+<char>(const dsTSTRING<char>&, const char*) @ 0x823B5518
// Concatenation with a C-string right operand: build a fresh string (returned by value / sret)
// that starts by sharing `a`'s buffer (ref-bump), then appends the NUL-terminated `b` at its end
// via the public Insert(pos, const char*, len=-1) overload. A null `b` becomes the empty string.
template<>
dsTSTRING<char> operator+<char>(const dsTSTRING<char> &a, const char *b)
{
    if (!b)
        b = empty_string;
    dsTSTRING<char> result;
    dsTSTRING_BUF_HEADER<char> *aBuffer = a.pBuffer;
    result.pBuffer = a.pBuffer;
    ++aBuffer->refCount;
    result.Insert(result.pBuffer->strLen, b, -1);
    return result;
}
