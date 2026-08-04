#include "dsAFFIX_STRING.h"

// dsAFFIX_STRING::dsAFFIX_STRING(const dsTSTRING<char>&) @ 0x8252D2E0
// `str` default-member-inits to the shared empty-string singleton, then the body assigns `initial`
// over it via operator= (self-checked release of the just-adopted empty buffer, then share+bump
// `initial`'s buffer).
dsAFFIX_STRING::dsAFFIX_STRING(const dsTSTRING<char> &initial)
{
    str = initial;
}
