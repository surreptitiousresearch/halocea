#include "dsAFFIX_READER.h"

// dsAFFIX_READER::dsAFFIX_READER(const dsTSTRING<char>&) @ 0x82A75D58
// Share `affixes`'s buffer (copy-construct, bumping its refcount) into affixStr, start with no
// recorded sub-strings, then parse it.
dsAFFIX_READER::dsAFFIX_READER(const dsTSTRING<char> &affixes)
    : affixStr(affixes)
{
    this->nSubStr = 0;
    this->curSubStr = -1;
    this->Init();
}
