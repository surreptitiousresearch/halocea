#include "headers/ws/vid/vidDIP.h"

// @0x826697F0 -- dipIDList's ~dsVECTOR releases its backing store (delegated to the member's own
// destructor); `name`'s buffer reference is dropped explicitly, matching the raw refcount
// decrement the decompiler shows (dsTSTRING<char>'s own ~dsTSTRING is boundary-only, so this
// mirrors its documented behavior directly rather than relying on it).
vidDIP::~vidDIP()
{
    // dipIDList destructs automatically (member dtor) after this body runs.
}
