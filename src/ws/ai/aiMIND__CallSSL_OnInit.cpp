#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_ssl_boundary.h"
#include "../../headers/ws/ai/aiPLANNER_boundary.h" // gsAppState
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

// aiMIND::CallSSL_OnInit @ 0x832299B0
// ?CallSSL_OnInit@aiMIND@@QAAXXZ
//
// Fire the unit's SSL "OnInit" script event for a live brain. Skipped for dead brains and while the
// SSL-init-suppressed app-state bit (gsAppState 0x8000000) is set. Before dispatching, stamp the
// script object's debug name from the brain's instance name; the whole body is timed by a named
// profiling counter.
void aiMIND::CallSSL_OnInit()
{
    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    if (brain->IsDead() || ((~gsAppState >> 27) & 1) == 0)
        return;

    // Named per-call-site profiling counter (Meyers singleton — the original guards a file-scope
    // global with a one-time flag and an atexit destructor, which a function-local static matches).
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/init/SSL_OnInit", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeTimer(&localTimeCounter);

    dsTSTRING<char> dbgName;
    dbgName.pBuffer = nullptr;
    dbgName.UnsafeInit(brain->names.nameInst, -1, 0);
    brain->sslObject.SetDbgName(dbgName);
    // dbgName's destructor releases its buffer at scope end (the decompiler's inlined
    // refcount-decrement/dlFree tail).

    // RAII: the returned sslERROR temporary releases itself at end of statement.
    brain->sslObject.HandleEvent(aiBRAIN_DESC_SslEvOnInit(brain->spDesc.pointee), 0, nullptr);
}
