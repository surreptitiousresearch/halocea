#include "dsTSTRING.h"

// dsTSTRING<char>::CStr @ 0x823B0930
// Return the character data inside the shared buffer header (buf + 0xC).
template<>
const char *dsTSTRING<char>::CStr() const
{
    return this->pBuffer->str;
}
