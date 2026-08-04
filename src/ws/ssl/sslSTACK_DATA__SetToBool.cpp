#include "sslSTACK_DATA.h"

// 0x82AF06F8 — sslSTACK_DATA::SetToBool.
sslSTACK_DATA &sslSTACK_DATA::SetToBool(bool val)
{
    Cleanup();
    bool_val = val;
    type = TYPE_BOOL;
    return *this;
}
