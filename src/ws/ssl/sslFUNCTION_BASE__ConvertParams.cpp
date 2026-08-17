/* ?ConvertParams@sslFUNCTION_BASE@@IAA?AVsslERROR@@PAVsslOBJECT@@HAAVsslSTACK@@@Z @0x82AD9870 */
#include "sslFUNCTION_BASE.h"
#include "sslFUNCTION_DECL.h"
#include "sslSTACK.h"
#include "sslSTACK_DATA.h"
#include "sslOBJECT.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsCONVERTION_TYPE.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82AD9870 — sslFUNCTION_BASE::ConvertParams. Normalise the `argc` call arguments sitting on the VM
// operand stack to this function's declared parameter list: append defaults for missing trailing
// params, drop surplus args, resolve every argument to a concrete value, then coerce each to its
// declared type. Returns sslERROR by value (sret): the decompiler swaps this/result (r3 = return slot
// sslERROR*, r4 = this sslFUNCTION_BASE*) and mistypes pSelf as dsTSTRING<char>*. Protected (IAA).
sslERROR sslFUNCTION_BASE::ConvertParams(sslOBJECT *pSelf, int argc, sslSTACK &stack)
{
    sslERROR err;
    int numParams = this->pDecl->param.nElem;
    // Base index of the argument block; invariant across the push/pop below (both move nElem and argc).
    int paramBase = stack.mainStack.nElem - argc;

    // Supply default values for any missing trailing parameters.
    while (argc < numParams)
    {
        stack.Push().SetToDsData(this->pDecl->param[argc].defValue);
        ++argc;
        numParams = this->pDecl->param.nElem;
    }
    // Drop any surplus arguments.
    while (argc > numParams)
    {
        stack.Pop();
        --argc;
        numParams = this->pDecl->param.nElem;
    }

    // Pass 1: materialise every argument slot into a concrete value (resolve reference kinds).
    for (int i = 0; i < numParams; ++i)
    {
        err = stack.At(paramBase + i).ConvertToDsData();
        if (err.id)
            return err;
    }

    // Pass 2: coerce each argument to its declared parameter type.
    for (int i = 0; i < numParams; ++i)
    {
        sslSTACK_DATA &slot = stack.At(paramBase + i);
        dsDATA argValue;
        argValue.type = nullptr;
        err = slot.GetAsDsData(argValue);
        if (err.id)
        {
            if (argValue.type)
                argValue.type->Destroy(&argValue.storage);
            return err;
        }

        const sslFUNCTION_DECL::PARAM &param = this->pDecl->param[i];
        if (argValue.type != param.pType && param.pType)
        {
            if (!argValue.Convert(param.pType, DSD_CONV_PARAM))
            {
                const char *gotName  = argValue.type ? argValue.type->GetDbgName() : "";
                const char *wantName = param.pType->GetDbgName();
                dsTSTRING<char> objName = pSelf->GetDbgName();
                const char *funcName = this->pDecl->name.CStr();
                dsTSTRING<char> msg;
                err = sslERROR(SSL_ERR_RUNTIME,
                               *dsSPrintf(&msg, "invalid '$%s.%s' parameter %i type (got <%s>, want <%s>)",
                                          objName.CStr(), funcName, i + 1, gotName, wantName),
                               0);
                if (argValue.type)
                    argValue.type->Destroy(&argValue.storage);
                return err;
            }
            slot.SetToDsData(argValue);
        }

        if (argValue.type)
            argValue.type->Destroy(&argValue.storage);
    }

    return err; // no-error
}
