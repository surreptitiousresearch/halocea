#include "dsAFFIX_STRING.h"

// dsAFFIX_STRING::operator= @ 0x823B10D0
// Copy-assign: forwards straight to dsTSTRING<char>::operator= on `str` (self-check, release the
// old buffer, share+bump the new one).
dsAFFIX_STRING &dsAFFIX_STRING::operator=(const dsAFFIX_STRING &that)
{
    str = that.str;
    return *this;
}
