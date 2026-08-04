#include "dsTSTRING.h"
#include <string.h>

// dsTSTRING<char>::Insert @ 0x823B12B0
// Make room for str at pos (grow-and-shift overload), then copy its characters in.
template<>
dsTSTRING<char> &dsTSTRING<char>::Insert(int pos, const dsTSTRING<char> &str)
{
    int insertLen = str.pBuffer->strLen;
    this->Insert(pos, insertLen); // grow/shift overload (boundary)
    memcpy(&this->pBuffer->str[pos], str.pBuffer->str, insertLen);
    return *this;
}
