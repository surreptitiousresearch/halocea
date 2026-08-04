#include "objOBJ.h"
#include "obj_boundary.h"
#include "../ds/dsAFFIX_READER.h"
// 0x82684D60  ?HaveAffix@objOBJ@@QBAHPBDH@Z
// Build a temporary affix reader over this object's affix string and test for `aff`.
// The reader shares the affix string's ref-counted buffer, so its destructor is inlined
// here: drop the refCount and free the buffer once it reaches zero.

int objOBJ::HaveAffix(const char *aff, int asSubString) const
{
    // affixes is dsAFFIX_STRING (dsTSTRING<char> at offset 0); reader ctor takes dsTSTRING<char>
    dsAFFIX_READER reader(*(const dsTSTRING<char> *)&affixes);
    int found = reader.HaveAffix(aff, asSubString);

    dsTSTRING_BUF_HEADER<char> *buffer = reader.affixStr.pBuffer;
    if (--buffer->refCount == 0)
        dlFree(buffer);
    return found;
}
