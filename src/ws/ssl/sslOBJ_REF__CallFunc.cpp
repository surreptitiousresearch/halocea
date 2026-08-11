#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "sslCLASS.h"
#include "sslCLASS_REF.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsTSTRING.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// @0x82523BE0 — sslOBJ_REF::CallFunc. Dispatch function/callback `idx` on the referenced object via
// its class (passing the object as self and `caller`'s object as the caller). When this ref is empty
// (the script was never parsed) build a formatted error instead. Returns sslERROR by value (sret):
// the decompiler swaps this/result (r3 = return slot sslERROR*, r4 = this sslOBJ_REF*). `caller` is
// passed by value; its object ref is released when the parameter is destroyed on return.
sslERROR sslOBJ_REF::CallFunc(int idx, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller)
{
    if (this->pObject)
    {
        return this->pObject->refClass.pClass->CallFunc(
            this->pObject, idx, argc, argv, retVal, caller.pObject);
    }

    dsTSTRING<char> msg;
    return sslERROR(SSL_ERR_GENERIC,
                    *dsSPrintf(&msg, "Script wasn't parsed. For programmers: func idx %i, obj <null>", idx),
                    0);
}
