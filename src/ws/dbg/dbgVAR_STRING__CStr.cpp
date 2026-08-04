#include "dbgVAR_STRING.h"

// dbgVAR_STRING::CStr @ 0x823C2358
// Return the string debug variable's current value as a NUL-terminated C string (borrows the
// shared dsTSTRING buffer -- valid only until the value next changes).
const char *dbgVAR_STRING::CStr() const
{
    const dsTSTRING<char> &value = this->GetValue();
    return value.CStr();
}
