#include "sslCLASS.h"
#include "sslSOURCE_REF.h"

// 0x82523120 — sslCLASS::GetSource. Returns a copy of this class's source-text reference by value
// (sret). The decompiler's `this`/`result` are swapped by the sret ABI (r3 = return slot, r4 =
// this); the real body copy-constructs an sslSOURCE_REF from this->source (offset 0x58).
sslSOURCE_REF sslCLASS::GetSource()
{
    return source;
}
