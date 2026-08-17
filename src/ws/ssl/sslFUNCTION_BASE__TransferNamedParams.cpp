/* ?TransferNamedParams@sslFUNCTION_BASE@@KA?AVsslERROR@@PAVsslCLASS@@HHAAVsslSTACK@@@Z @0x82ADA548 */
#include "sslFUNCTION_BASE.h"
#include "sslFUNCTION_DECL.h"
#include "sslCLASS.h"
#include "sslSTACK.h"
#include "sslSTACK_DATA.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82ADA548 — sslFUNCTION_BASE::TransferNamedParams. Reorder the `argc` named call arguments (values
// on the main operand stack, their names on the namedParams stack) into the declared parameter order,
// pushing them back positionally with defaults filled for omitted parameters. Errors on an unknown
// parameter name or a value that cannot be read. Static (KA), returns sslERROR by value (sret): the
// decompiler puts the return slot in r3.
//
// CAVEAT: the decompiler packs `argValues` (a dsVECTOR<dsDATA,8>) and `usedFlags` (a dsVECTOR<bool,8>)
// plus assorted scratch into one big sslCLASS[2] stack buffer with heavy field aliasing; untangled
// here into the two named local vectors whose RAII destructors reproduce the manual frees.
sslERROR sslFUNCTION_BASE::TransferNamedParams(sslCLASS *pClass, int idx, int argc, sslSTACK &stack)
{
    sslFUNCTION_DECL *decl = pClass->GetFunctionDecl(idx); // member-call form (decompiler flat form miscounts args)
    if (!decl)
    {
        dsSTRID name = pClass->GetSubElemName(idx);
        dsTSTRING<char> msg;
        return sslERROR(SSL_ERR_RUNTIME, *dsSPrintf(&msg, "'%s' is not a function", name.CStr()), 0);
    }

    int mainBase  = stack.mainStack.nElem - argc;    // base of the positional arg values
    int namedBase = stack.namedParams.nElem - argc;  // base of the argument names

    dsVECTOR<dsDATA, 8> argValues;
    argValues.Resize(argc);
    dsVECTOR<bool, 8> usedFlags;
    bool notUsed = false;
    usedFlags.Resize(argc, notUsed);

    // Read every positional argument value out of the stack; bail on the first read error.
    for (int i = 0; i < argc; ++i)
    {
        sslERROR readErr = stack.At(mainBase + i).GetAsDsData(argValues[i]);
        if (readErr.id)
            return readErr;
    }

    // Drop the positional args from the main stack; we re-push in declared order below.
    stack.mainStack.ShrinkResize(mainBase);

    for (int p = 0; p < decl->param.nElem; ++p)
    {
        const char *paramId = decl->param[p].name.id;

        // Locate this parameter's supplied value by matching its name against the argument names.
        int matchIdx = -1;
        for (int j = namedBase; j < stack.namedParams.nElem; ++j)
        {
            if (stack.namedParams[j].id == paramId)
            {
                matchIdx = j;
                break;
            }
        }

        if (matchIdx < 0)
        {
            stack.Push().SetToDsData(decl->param[p].defValue);
        }
        else
        {
            int argIdx = matchIdx - namedBase;
            stack.Push().SetToDsData(argValues[argIdx]);
            usedFlags[argIdx] = true;
        }
    }

    // Every supplied name must have matched a declared parameter.
    for (int k = 0; k < usedFlags.nElem; ++k)
    {
        if (!usedFlags[k])
        {
            const char *badName  = stack.namedParams[namedBase + k].CStr();
            const char *funcName = decl->name.CStr();
            dsTSTRING<char> msg;
            return sslERROR(SSL_ERR_RUNTIME,
                            *dsSPrintf(&msg, "unknown '%s' parameter '%s'", funcName, badName), 0);
        }
    }

    stack.namedParams.ShrinkResize(namedBase);
    return sslERROR();
}
