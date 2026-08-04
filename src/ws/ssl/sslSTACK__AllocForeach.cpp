#include "sslSTACK.h"
#include "sslFOREACH_DATA.h"
#include "sslCLASS_REF.h"

// 0x82AF4670 — sslSTACK::AllocForeach. Push a fresh foreach cursor: append a default-constructed
// sslFOREACH_DATA, then bind its class filter to `cls` (ref-counted assign) and reset lastObjIdx.
// `cls` is passed by value (its ref is released when the by-value parameter is destroyed at return).
void sslSTACK::AllocForeach(sslCLASS_REF cls)
{
    sslFOREACH_DATA newCursor; // checkClass default-constructs to an empty ref
    sslFOREACH_DATA &pushed = foreachStack.PushBack(newCursor);
    pushed.checkClass = cls;
    pushed.lastObjIdx = -1;
}
