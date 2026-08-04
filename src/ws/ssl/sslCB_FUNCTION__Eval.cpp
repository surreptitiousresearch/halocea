#include "sslCB_FUNCTION.h"
#include "sslFUNCTION_DECL.h"
#include "sslOBJECT.h"
#include "sslOBJ_REF.h"
#include "sslCB_HOST.h"
#include "sslCLASS_REF.h"
#include "sslSYSTEM.h"
#include "sslSTACK.h"
#include "sslSTACK_DATA.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsCONVERTION_TYPE.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary
// Global placement copy-construct helper (New<T>(dst, src)); not reversed here.
template<class T> T *New(T *dst, const T *src); // boundary

// Scratch parameter stack shared by native callback dispatch. boundary — defined elsewhere.
extern dsVECTOR<dsDATA, 8> cbParamsStack;
// Debug call-stack instrumentation globals. boundary — defined elsewhere.
extern bool dbg_sslCallStack;
extern dsVECTOR<dsTSTRING<char>, 8> sslCallStack;
extern dsVECTOR<sslOBJECT *, 8>     sslCallStackObj;
extern sslSYSTEM *gsSslSystem;

// 0x82AE0D68 — sslCB_FUNCTION::Eval. Invoke this native (C++) callback on `pSelf`. Returns sslERROR by
// value (sret): the decompiler swaps this/result (r3 = return slot sslERROR*, r4 = this sslCB_FUNCTION*),
// so its `this->pDecl/pClassOfDefinition/stateFunc` writes are really the returned error's id/desc/tags,
// and `result->id/tags` reads are this function's pDecl / flag word.
//
// Arguments come either directly (`argv` non-null, padded with parameter defaults) or off the VM stack
// (ConvertParams then copied into cbParamsStack). After optional logging and per-parameter type checks,
// the callback is dispatched in its object-scoped or global form (stateFunc bit 0x4), the return value
// is coerced to the declared return type, and the scratch/param stacks are restored.
//
// CAVEAT: the native call itself is emitted by the decompiler as a register-only indirect call
// (`((void(*)())binding)()`); it is reconstructed here as the typed pCbObject/pCbGlobal invocation from
// the DB-verified binding union. Object guard state (state bit 0x10) short-circuits to a default return.
sslERROR sslCB_FUNCTION::Eval(sslOBJECT *pSelf, int argc, dsDATA *argv, dsDATA &retVal, sslOBJECT *pCaller)
{
    sslERROR err;
    bool argsProvidedDirectly = (argv != nullptr);
    int savedCbStackSize = cbParamsStack.nElem;
    sslSYSTEM *ssl = pSelf->refClass.GetSslSystem();
    int numParams = this->pDecl->param.nElem;

    if (argv)
    {
        // Direct arguments: pad with parameter defaults when the caller under-supplied.
        if (argc < numParams)
        {
            cbParamsStack.Resize(numParams + savedCbStackSize);
            dsDATA *dst = &cbParamsStack.pData[savedCbStackSize];
            for (int i = 0; i < argc; ++i)
                dst[i].StoreValue(argv[i]);
            for (int i = argc; i < numParams; ++i)
                New<dsDATA>(&dst[i], &this->pDecl->param[i].defValue);
            argv = dst;
            argc = numParams;
        }
    }
    else
    {
        // Stack arguments: normalise on the operand stack, then copy into the scratch param stack.
        sslSTACK *pStack = &ssl->stack;
        int paramBase = pStack->mainStack.nElem - argc;
        this->ConvertParams(pSelf, argc, *pStack); // return value intentionally discarded (re-fetched below)

        cbParamsStack.Resize(numParams + savedCbStackSize);
        argv = &cbParamsStack.pData[savedCbStackSize];
        for (int i = 0; i < numParams; ++i)
        {
            err = pStack->At(i + paramBase).GetAsDsData(argv[i]);
            if (err.id)
                break;
        }
        pStack->mainStack.ShrinkResize(paramBase);
        argc = numParams;
    }

    ++ssl->logFuncActive;

    // Optionally log the call.
    bool logToSystem = (!ssl->disFuncLog && ssl->logFuncActive > 0);
    if (logToSystem || dbg_sslCallStack)
    {
        dsTSTRING<char> logStr;
        dsSPrintf(&logStr, "func $%s.%s (", pSelf->GetDbgName().CStr(), this->pDecl->name.CStr());
        logStr.Reserve(256);
        for (int i = 0; i < argc; ++i)
        {
            if (i)
                logStr.Insert(logStr.pBuffer->strLen, ", ", -1);
            dsTSTRING<char> argStr = argv[i].GetDbgString();
            logStr.Insert(logStr.pBuffer->strLen, argStr);
        }
        logStr.Insert(logStr.pBuffer->strLen, ") (func, c++)", -1);

        if (logToSystem)
            ssl->Log(logStr);
        if (dbg_sslCallStack)
        {
            sslCallStack.PushBack(logStr);
            sslCallStackObj.PushBack(pSelf);
        }
    }

    ssl->LogTab();

    // Direct-argument path type-checks each parameter (the stack path already coerced via ConvertParams).
    if (argsProvidedDirectly && !err.id)
    {
        for (int i = 0; i < numParams; ++i)
        {
            const dsDATA_TYPE *paramType = this->pDecl->param[i].pType;
            if (argv[i].type != paramType && paramType && !argv[i].Convert(paramType, DSD_CONV_PARAM))
            {
                dsTSTRING<char> objName = pSelf->GetDbgName();
                dsTSTRING<char> msg;
                err = sslERROR(SSL_ERR_RUNTIME,
                               *dsSPrintf(&msg, "invalid function '$%s.%s' parameter %i type ",
                                          objName.CStr(), this->pDecl->name.CStr(), i + 1),
                               0);
                break;
            }
        }
    }

    // Dispatch the callback (unless an error was already raised).
    if (!err.id)
    {
        if (((pSelf->state.state >> 4) & 1) != 0)
        {
            // Object is in the guarded state: skip the call and produce a default return value.
            const dsDATA_TYPE *retType = this->pDecl->pRetType;
            if (retType)
            {
                dsDATA def;
                def.type = retType;
                retType->Construct(&def.storage);
                retVal.StoreValue(def);
                if (def.type)
                    def.type->Destroy(&def.storage);
            }
            else
            {
                dsDATA empty;
                empty.type = nullptr;
                retVal.StoreValue(empty);
            }
        }
        else
        {
            sslOBJ_REF callerRef;
            sslOBJ_REF selfRef;
            callerRef._SetObjectPtr(pCaller);
            selfRef._SetObjectPtr(pSelf);

            if ((this->stateFunc & 4) != 0)
            {
                // Global (object-independent) binding.
                this->pCbGlobal(selfRef, argc, argv, &retVal, callerRef);
            }
            else
            {
                sslCB_HOST *cbHost = pSelf->GetCbHost();
                if (cbHost)
                {
                    this->pCbObject(cbHost, argc, argv, &retVal, callerRef);
                }
                else
                {
                    dsTSTRING<char> objName = pSelf->GetDbgName();
                    dsTSTRING<char> msg;
                    err = sslERROR(SSL_ERR_RUNTIME,
                                   *dsSPrintf(&msg, "calling function '%s' of the dead object '$%s'",
                                              this->pDecl->name.CStr(), objName.CStr()),
                                   0);
                }
            }
        }
    }

    // Coerce the return value to the declared return type.
    if (!err.id)
    {
        const dsDATA_TYPE *retType = this->pDecl->pRetType;
        if (retType && !retVal.Convert(retType, DSD_CONV_PARAM))
        {
            const char *gotName  = retVal.type ? retVal.type->GetDbgName() : "null";
            const char *wantName = retType->GetDbgName();
            dsTSTRING<char> objName = pSelf->GetDbgName();
            dsTSTRING<char> msg;
            err = sslERROR(SSL_ERR_RUNTIME,
                           *dsSPrintf(&msg, "can't convert '$%s.%s' return value from type <%s> to type <%s>",
                                      objName.CStr(), this->pDecl->name.CStr(), gotName, wantName),
                           0);
        }
    }

    // On error, snapshot the call stack into the error call stack.
    if (err.id)
    {
        for (int i = 0; i < sslCallStack.nElem; ++i)
            gsSslSystem->errorCallStack.PushBack(sslCallStack[i]);
    }

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

    // Restore the scratch parameter stack.
    cbParamsStack.Resize(savedCbStackSize);
    return err;
}
