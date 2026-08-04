#include "sslSTACK_DATA.h"

// 0x82AF06B0 — sslSTACK_DATA::SetToInt. Returns *this (AAV1) so callers can chain.
sslSTACK_DATA &sslSTACK_DATA::SetToInt(int val)
{
    Cleanup();
    int_val = val;
    type = TYPE_INT;
    return *this;
}
