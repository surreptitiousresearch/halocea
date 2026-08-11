#include "../headers/ws/ds/dsERROR_HANDLER.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/hcex/haloENGINE_CONTROL.h"

// @0x84101BC8 — anonymous file-static function registered via atexit() the first time
// dsERROR_HANDLER<haloENGINE_CONTROL>::GetErrorStr() runs; destroys the function-local static
// dsTSTRING error slot ('err') at process shutdown.
// (Compiler-synthesized name: `dsERROR_HANDLER<haloENGINE_CONTROL>::GetErrorStr'::`2'::`dynamic
// atexit destructor for 'err''`.)
// DEVIATION: the decompiler expands the dsTSTRING destructor inline (processor-lock check +
// refcount decrement + dlFree of the shared buffer); the ~dsTSTRING() call is the source-level
// equivalent, reached here through the GetErrorStr() reference to the same static.
static void GetErrorStr_dynamic_atexit_destructor_for_err()
{
    dsERROR_HANDLER<haloENGINE_CONTROL>::GetErrorStr().~dsTSTRING();
}
