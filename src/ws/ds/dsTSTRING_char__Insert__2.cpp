#include "dsTSTRING.h"

// dsTSTRING<char>::Insert @ 0x824F9D80  (fill overload; overload #2 by ascending address)
// Make room for `num` characters at `pos` (grow-and-shift overload), then fill them with `c`.
template<>
dsTSTRING<char> &dsTSTRING<char>::Insert(int pos, char c, int num)
{
    this->Insert(pos, num); // grow/shift overload (boundary)
    for (int i = 0; i < num; ++i)
        this->pBuffer->str[pos + i] = c;
    return *this;
}
