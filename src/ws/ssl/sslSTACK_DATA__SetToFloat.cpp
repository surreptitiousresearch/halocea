#include "sslSTACK_DATA.h"

// 0x82AF0740 — sslSTACK_DATA::SetToFloat. Param is float per the mangle (M); the decompiler's
// `double val` is the float-slot-skip ABI artifact.
sslSTACK_DATA &sslSTACK_DATA::SetToFloat(float val)
{
    Cleanup();
    float_val = val;
    type = TYPE_FLOAT;
    return *this;
}
