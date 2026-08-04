#include "sslSYSTEM.h"
#include "sslOBJ_REF.h"

// 0x825C9A48 — sslSYSTEM::GetGlobalObj. Returns a copy of the shared global object reference by
// value (sret). The decompiler's `this`/`result` are swapped by the sret ABI (r3 = return slot,
// r4 = this); the real body copy-constructs an sslOBJ_REF from this->globalObj (offset 152).
sslOBJ_REF sslSYSTEM::GetGlobalObj()
{
    return globalObj;
}
