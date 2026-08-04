#include "sslCLASS.h"
#include "sslCLASS_ELEMENT.h"
#include "sslSCRIPTED_FUNCTION.h"
#include "sslOBJECT.h"
#include "sslCLASS_REF.h"
#include "sslSYSTEM.h"
#include "sslSTACK.h"
#include "sslSTACK_DATA.h"
#include "sslCALL_FUNC_COUNTER.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"

// The most-recently-entered script function name, kept for crash diagnostics. boundary — global.
extern dsTSTRING<char> gDebugFuncName;

// 0x82A39A18 — sslCLASS::CallScriptedFunc. Returns sslERROR by value (sret): the decompiler swaps
// this/result (r3 = return slot sslERROR*, r4 = this sslCLASS*). Push the call arguments onto the
// owning system's stack, record the function name, then evaluate the compiled function.
//
// CAVEAT: the decompiler mis-sizes the Eval sret buffer as a full sslSCRIPTED_FUNCTION and reads its
// desc-string teardown as `pClassOfDefinition`; it is really the standard by-value sslERROR return
// of Eval, modeled cleanly here as `return fn->Eval(...)`.
sslERROR sslCLASS::CallScriptedFunc(sslOBJECT *pSelf, int idx, int argc, dsDATA *argv,
                                    dsDATA &retVal, sslOBJECT *pCaller)
{
    sslCALL_FUNC_COUNTER counter(this->pSslSystem);

    sslSCRIPTED_FUNCTION *fn = this->elements[idx].pSslFunc;

    // Run unless the object is still constructing and the function is not flagged run-during-construct.
    if ((pSelf->state.state & 1) == 0 || (fn->stateFunc & 2) != 0)
    {
        sslSTACK *pStack = &pSelf->refClass.GetSslSystem()->stack;

        if (argv && argc > 0)
        {
            for (int n = argc; n; --n, ++argv)
                pStack->Push().SetToDsData(*argv);
        }

        dsTSTRING<char> funcName;
        funcName.UnsafeInit(this->elements[idx].name.CStr(), -1, 0);
        gDebugFuncName = funcName;

        return this->elements[idx].pSslFunc->Eval(pSelf, argc, &retVal, pCaller);
    }

    return sslERROR();
}
