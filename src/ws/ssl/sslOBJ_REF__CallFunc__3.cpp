#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "sslCLASS.h"
#include "sslCLASS_REF.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82526578 — sslOBJ_REF::CallFunc (dsSTRID-name overload __3). Returns sslERROR by value (sret):
// the decompiler swaps this/result (r3 = return slot sslERROR*, r4 = this sslOBJ_REF*). Resolve
// `name` to an element index on the object's class, then dispatch it; error when the ref is empty
// or the name is unknown. `caller` is passed by value (its ref is released on return).
sslERROR sslOBJ_REF::CallFunc(dsSTRID name, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller)
{
    if (this->pObject)
    {
        int idx = this->pObject->refClass.pClass->GetSubNameIdx(name);
        if (idx == -1)
        {
            dsTSTRING<char> dbgName = this->GetDbgName();
            dsTSTRING<char> msg;
            return sslERROR(SSL_ERR_UNKNOWN_NAME,
                            *dsSPrintf(&msg, "function '%s' not found in object '%s'",
                                       name.CStr(), dbgName.pBuffer->str),
                            0);
        }
        return this->pObject->refClass.pClass->CallFunc(
            this->pObject, idx, argc, argv, retVal, caller.pObject);
    }

    dsTSTRING<char> msg;
    return sslERROR(SSL_ERR_GENERIC,
                    *dsSPrintf(&msg, "calling function '%s' of the <null> object", name.CStr()),
                    0);
}
