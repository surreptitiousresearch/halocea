#include "sslDELAYED_FUNCTION.h"
#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "sslCLASS.h"
#include "sslCLASS_REF.h"
#include "sslSYSTEM.h"
#include "sslSTACK.h"
#include "sslSTACK_DATA.h"
#include "sslFUNCTION_DECL.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsSTATE_MGR.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82AF4E18 — sslDELAYED_FUNCTION::Setup. (Re)initialise this record for a queued call: bind
// self/funcIdx/caller/timer/group, snapshot the current SyncContext, and pop `argc` arguments off the
// stack into `params`. Returns sslERROR by value (sret).
//
// CAVEAT: the `float timeOfs` parameter triggers the PPC float-slot-skip ABI, so the decompiler
// mis-shifts the trailing GPR args (its `stack`=argc, `group`=stack, `a29`=group) and mistypes the
// sret/this pair. The true signature (self, funcIdx, caller, timeOfs, argc, stack, group) is recovered
// from the field stores. The diagnostic log format string is also garbled by the same skip and is
// reconstructed best-effort.
sslERROR sslDELAYED_FUNCTION::Setup(sslOBJ_REF self, int funcIdx, sslOBJ_REF caller, float timeOfs,
                                    int argc, sslSTACK &stack, dsSTRID group)
{
    sslERROR err;
    sslSYSTEM *ssl = self.pObject->refClass.GetSslSystem();

    // The "SyncContext" state id is registered once, lazily; snapshot its current value.
    static int syncContextStateId = gStateMgr->RegisterState(dsSTRID("SyncContext", 0), false);
    this->syncContext.StoreValue(*gStateMgr->GetState(syncContextStateId));

    this->self = self;
    this->funcIdx = funcIdx;
    this->caller = caller;
    this->timeLeft = timeOfs;
    this->pauseLocks = 0;
    this->group = group;
    this->params.Resize(argc);

    if (argc > 0)
    {
        // Snapshot arguments off the stack top into params, filling the last param first.
        int filled = 0;
        int paramIdx = argc - 1;
        bool failed = false;
        while (true)
        {
            err = stack.GetTop().GetAsDsData(this->params[paramIdx]);
            if (stack.mainStack.nElem)
                stack.mainStack.ShrinkResize(stack.mainStack.nElem - 1);
            if (err.id)
            {
                failed = true;
                break;
            }
            ++filled;
            --paramIdx;
            if (filled >= argc)
                break;
        }

        if (failed)
        {
            // Drop the arguments not yet consumed and mark the record invalid.
            for (int rem = argc - filled; rem; --rem)
            {
                if (stack.mainStack.nElem)
                    stack.mainStack.ShrinkResize(stack.mainStack.nElem - 1);
            }
            this->funcIdx = -1;
            return err;
        }
    }

    // Diagnostic log of the queued call, when function logging is active.
    if (!ssl->disFuncLog && ssl->logFuncActive > 0)
    {
        sslOBJECT *obj = self.pObject;
        sslCLASS *cls = self.GetClass().pClass;
        if (obj && cls)
        {
            sslFUNCTION_DECL *decl = cls->GetFunctionDecl(this->funcIdx); // member-call form
            if (decl)
            {
                dsTSTRING<char> logStr;
                dsSPrintf(&logStr, "delayed call $%s.%s in %gs (",
                          self.GetDbgName().CStr(), decl->name.CStr(), timeOfs);
                logStr.Reserve(256);
                for (int i = 0; i < argc; ++i)
                {
                    if (i)
                        logStr.Insert(logStr.pBuffer->strLen, ", ", -1);
                    dsTSTRING<char> argStr = this->params[i].GetDbgString();
                    logStr.Insert(logStr.pBuffer->strLen, argStr);
                }
                logStr.Insert(logStr.pBuffer->strLen, ")", -1);
                ssl->Log(logStr);
            }
        }
    }

    return err;
}
