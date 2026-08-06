#include "dsSTRID.h"

extern "C" const char *dsStrIdOfEmptyId; // ds global — the interned id of the empty string

// dsSTRID::dsSTRID() @ 0x825280E0
// Default: adopt the process-wide interned id of the empty string.
dsSTRID::dsSTRID()
{
    this->id = dsStrIdOfEmptyId;
}
