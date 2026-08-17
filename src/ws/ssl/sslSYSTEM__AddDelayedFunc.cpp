/* ?AddDelayedFunc@sslSYSTEM@@QAA?AVsslERROR@@VsslOBJ_REF@@H0MHAAVsslSTACK@@VdsSTRID@@@Z @0x825D0358 */
#include "sslSYSTEM.h"
#include "sslDELAYED_FUNCTION.h"
#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "sslSTACK.h"
#include "sslERROR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x825D0358 — sslSYSTEM::AddDelayedFunc. Returns sslERROR by value (sret): the decompiler swaps
// this/result (r3 = return slot sslERROR*, r4 = this sslSYSTEM*). `timeOfs` is declared float but
// arrives widened to double in the decompile (float-slot ABI). Queue a new delayed-call record and
// forward to sslDELAYED_FUNCTION::Setup. `obj` and `caller` are passed by value (released on return).
sslERROR sslSYSTEM::AddDelayedFunc(sslOBJ_REF obj, int funcIdx, sslOBJ_REF caller, float timeOfs,
                                   int argc, sslSTACK &stack, dsSTRID group)
{
    if (!obj.pObject)
    {
        dsTSTRING<char> msg;
        return sslERROR(SSL_ERR_RUNTIME, *dsSPrintf(&msg, "delayed call on <null> object"), 0);
    }

    if (((obj.pObject->state.state >> 2) & 1) == 0) // bit 0x4: object supports delayed calls
    {
        dsTSTRING<char> name = obj.GetName();
        dsTSTRING<char> msg;
        return sslERROR(SSL_ERR_RUNTIME,
                        *dsSPrintf(&msg, "delayed calls on object '%s' is not supported", name.pBuffer->str),
                        0);
    }

    obj.pObject->state.state |= 8u; // mark "delayed call pending"

    sslDELAYED_FUNCTION *df = new sslDELAYED_FUNCTION(); // binary uses debug operator new(size,file,line)
    this->delayedFuncs.PushBack(df);

    sslOBJ_REF callerCopy(caller);
    sslOBJ_REF objCopy(obj);
    return df->Setup(objCopy, funcIdx, callerCopy, timeOfs, argc, stack, group);
}
