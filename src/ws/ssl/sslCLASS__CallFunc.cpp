#include "sslCLASS.h"
#include "sslCLASS_ELEMENT.h"
#include "sslOBJECT.h"
#include "sslERROR.h"
#include "sslCALL_FUNC_COUNTER.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
#include "../ap/apDATA_TRACKER.h"
#include "../ap/apCOUNTER_TIME.h"
#include "../os/osTIMER2.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary
// bounded snprintf into a caller buffer; not reversed here.
extern "C" int _snprintf_0(char *buf, unsigned int size, const char *fmt, ...); // boundary
// boundary — the process-wide descriptor for the null/empty value type.
struct NULL_TYPE;
template<class T> struct dsDATA_TYPE_STORAGE { static const dsDATA_TYPE *dataType; };

// Profiling instrumentation (boundary): a static recurring time counter for CallFunc, the SSL call
// tracker, and the recursion-depth counter.
struct apCOUNTER_TIME_RECUR;
extern apCOUNTER_TIME_RECUR sslCallFuncTimeCounter; // boundary — static "ssl/sslCLASS::CallFunc" counter
extern apDATA_TRACKER sslTracker;                   // boundary — SSL call statistics tracker
extern int cbdeep;                                  // boundary — CallFunc recursion depth

// @0x82A3BA50 — sslCLASS::CallFunc. Public function dispatch. Returns sslERROR by value (sret): the
// decompiler swaps this/result (r3 = return slot sslERROR*, r4 = this sslCLASS*) and mistypes pSelf as
// dsTSTRING<char>*. Routes element `idx` to the native-callback path (kind 0), the scripted-function
// path (kind 1), or reports "is not a function" for any other kind; out-of-range indices report a
// range error.
//
// CAVEAT: the profiling scaffolding (sslCALL_FUNC_COUNTER, the apCOUNTER_TIME_CALL_WRP static-counter
// scope, the osTIMER2 call timer, cbdeep and the apDATA_TRACKER series updates) is reconstructed
// best-effort as boundary calls; the raw `sslTracker.file.fp` open test is modeled as
// apDATA_TRACKER::IsRecording().
sslERROR sslCLASS::CallFunc(sslOBJECT *pSelf, int idx, int argc, dsDATA *argv, dsDATA *retVal, sslOBJECT *pCaller)
{
    sslCALL_FUNC_COUNTER counter(this->pSslSystem);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_RECUR> timeScope(&sslCallFuncTimeCounter);

    osTIMER2 callTimer;
    callTimer.Reset();
    ++cbdeep;
    if (sslTracker.IsRecording())
        callTimer.Reset();

    sslERROR result;

    if (idx >= 0 && idx <= this->elements.nElem)
    {
        // Give scripted callees a scratch return slot when the caller does not want the result.
        dsDATA defaultRet;
        const dsDATA_TYPE *nullType = dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType;
        defaultRet.type = nullType;
        nullType->Construct(&defaultRet.storage);
        if (!retVal)
            retVal = &defaultRet;

        sslCLASS_ELEMENT &elem = this->elements[idx];
        if (elem.type == sslCLASS_ELEMENT::TYPE_CB_FUNC)
        {
            result = this->CallCbFunc(pSelf, idx, argc, argv, *retVal, pCaller);
        }
        else if (elem.type == sslCLASS_ELEMENT::TYPE_SSL_FUNC)
        {
            result = this->CallScriptedFunc(pSelf, idx, argc, argv, *retVal, pCaller);
        }
        else
        {
            dsTSTRING<char> objName = pSelf->GetDbgName();
            dsTSTRING<char> msg;
            result = sslERROR(SSL_ERR_RUNTIME,
                              *dsSPrintf(&msg, "'%s' is not a function in for object '%s'",
                                         elem.name.CStr(), objName.CStr()),
                              0);
        }

        if (sslTracker.IsRecording())
        {
            float elapsed = callTimer.Snapshot();
            char nameBuf[0x400];

            _snprintf_0(nameBuf, 0x400, "CallFunc/%s/%s/time", pSelf->GetDbgName().CStr(), elem.name.CStr());
            sslTracker.AddValue(nameBuf, elapsed);
            _snprintf_0(nameBuf, 0x400, "CallFunc/%s/%s/calls", pSelf->GetDbgName().CStr(), elem.name.CStr());
            sslTracker.AddValue(nameBuf, 1.0f);

            if (cbdeep == 1)
            {
                sslTracker.AddValue("CallFunc/_sum/time", elapsed);
                sslTracker.AddValue("CallFunc/_sum/calls", 1.0f);
            }
        }

        if (defaultRet.type)
            defaultRet.type->Destroy(&defaultRet.storage);
    }
    else
    {
        dsTSTRING<char> objName = pSelf->GetDbgName();
        dsTSTRING<char> msg;
        result = sslERROR(SSL_ERR_RUNTIME,
                          *dsSPrintf(&msg, "index %i is not in range for object '%s'", idx, objName.CStr()),
                          0);
    }

    --cbdeep;
    return result;
}
