#include "dsSTRID.h"

// dsSTRID::CStr @ 0x82528108
// Return the interned C-string pointer stored at offset 0.
const char *dsSTRID::CStr() const
{
    return this->id;
}
