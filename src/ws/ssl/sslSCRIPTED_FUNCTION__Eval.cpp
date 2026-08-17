/* ?Eval@sslSCRIPTED_FUNCTION@@QAA?AVsslERROR@@PAVsslOBJECT@@HPAVdsDATA@@0@Z @0x82ADF2A8 */
#include "sslSCRIPTED_FUNCTION.h"
#include "sslFUNCTION_DECL.h"
#include "sslOBJECT.h"
#include "sslCLASS_REF.h"
#include "sslSYSTEM.h"
#include "sslSTACK.h"
#include "sslSTACK_DATA.h"
#include "sslSTACK_STATE.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsCONVERTION_TYPE.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// Debug call-stack instrumentation globals. boundary — defined elsewhere.
extern bool dbg_sslCallStack;                       // enables call-stack capture
extern dsVECTOR<dsTSTRING<char>, 8> sslCallStack;   // active script call frames (text)
extern dsVECTOR<sslOBJECT *, 8>     sslCallStackObj; // active script call frames (objects)
extern sslSYSTEM *gsSslSystem;                      // the active script system

// 0x82ADF2A8 — sslSCRIPTED_FUNCTION::Eval. Evaluate this compiled function on `pSelf` for `pCaller`,
// writing the result into `*retVal`. Returns sslERROR by value (sret): the decompiler swaps this/result
// (r3 = return slot sslERROR*, r4 = this sslSCRIPTED_FUNCTION*), so its `this->pDecl/pClassOfDefinition/
// stateFunc` writes are really the returned error's id/desc/tags; `result->id` reads are this function's
// pDecl. Snapshots the stack, converts params, optionally logs the call, runs the bytecode, coerces the
// return value to the declared type, and restores the stack.
sslERROR sslSCRIPTED_FUNCTION::Eval(sslOBJECT *pSelf, int argc, dsDATA *retVal, sslOBJECT *pCaller)
{
    sslERROR err;
    sslSYSTEM *ssl = pSelf->refClass.GetSslSystem();
    sslSTACK *pStack = &ssl->stack;

    // Save the stack cursor; the param block begins `argc` entries below the current top.
    sslSTACK_STATE savedState;
    pStack->GetState(savedState);
    savedState.mainStackSize -= argc;
    int paramBase = savedState.mainStackSize;

    err = this->ConvertParams(pSelf, argc, *pStack);

    int numParams = this->pDecl->param.nElem;
    ++ssl->logFuncActive;

    // Optionally log the call (to the system log and/or the debug call stack).
    bool logToSystem = (!ssl->disFuncLog && ssl->logFuncActive > 0);
    if (logToSystem || dbg_sslCallStack)
    {
        dsTSTRING<char> logStr;
        dsSPrintf(&logStr, "$%s.%s (", pSelf->GetDbgName().CStr(), this->pDecl->name.CStr());
        logStr.Reserve(256);
        for (int i = 0; i < numParams; ++i)
        {
            if (i)
                logStr.Insert(logStr.pBuffer->strLen, ", ", -1);
            dsDATA argVal;
            argVal.type = nullptr;
            pStack->At(i + paramBase).GetAsDsData(argVal);
            dsTSTRING<char> argStr = argVal.GetDbgString();
            logStr.Insert(logStr.pBuffer->strLen, argStr);
            if (argVal.type)
                argVal.type->Destroy(&argVal.storage);
        }
        logStr.Insert(logStr.pBuffer->strLen, ") (func, ssl)", -1);

        if (logToSystem)
            ssl->Log(logStr);
        if (dbg_sslCallStack)
        {
            sslCallStack.PushBack(logStr);
            sslCallStackObj.PushBack(pSelf);
        }
    }

    ssl->LogTab();

    // Run the compiled bytecode unless argument conversion already failed.
    if (!err.id)
        err = this->ExecCode(pSelf, *pStack, numParams, paramBase, retVal, pCaller);

    // Coerce the return value to the declared return type.
    if (!err.id && retVal)
    {
        const dsDATA_TYPE *retType = this->pDecl->pRetType;
        if (retType && !retVal->Convert(retType, DSD_CONV_PARAM))
        {
            const char *gotName  = retVal->type ? retVal->type->GetDbgName() : "null";
            const char *wantName = retType->GetDbgName();
            dsTSTRING<char> objName = pSelf->GetDbgName();
            const char *funcName = this->pDecl->name.CStr();
            dsTSTRING<char> msg;
            err = sslERROR(SSL_ERR_RUNTIME,
                           *dsSPrintf(&msg, "$%s.%s(): can't convert function return value from type <%s> to type <%s>",
                                      objName.CStr(), funcName, gotName, wantName),
                           0);
        }
    }

    // On error, snapshot the current call stack into the error call stack for diagnostics.
    if (err.id)
    {
        for (int i = 0; i < sslCallStack.nElem; ++i)
            gsSslSystem->errorCallStack.PushBack(sslCallStack[i]);
    }

    pStack->RestoreState(savedState);
    --ssl->logFuncActive;
    ssl->LogUntab();

    // Pop the frames pushed for this call.
    if (dbg_sslCallStack && sslCallStack.nElem > 0)
    {
        dsTSTRING<char> empty;
        empty.pBuffer = nullptr;
        empty.UnsafeInit("", -1, 0);
        sslCallStack.Back() = empty;
        sslCallStack.ShrinkResize(sslCallStack.nElem - 1);
    }
    if (dbg_sslCallStack && sslCallStackObj.nElem > 0)
    {
        sslCallStackObj.Back() = nullptr;
        sslCallStackObj.ShrinkResize(sslCallStackObj.nElem - 1);
    }

    return err;
}
