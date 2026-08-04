#include "sslSTACK.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsCONVERTION_TYPE.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82AF1C58 — sslSTACK::SetLocalVar. Returns sslERROR by value (sret): the decompiler swaps
// this/result (r3 = return slot sslERROR*, r4 = this sslSTACK*). Store `val` into local `idx`,
// converting to the local's declared type when it has one.
sslERROR sslSTACK::SetLocalVar(int idx, const dsDATA &val)
{
    if (idx >= 0 && idx < this->localsStack.nElem)
    {
        LOCAL_VAR &local = this->localsStack[idx];

        if (!local.pType || local.pType == val.type)
        {
            // Untyped local, or already the matching type: adopt the value directly.
            local.val.StoreValue(val);
            return sslERROR();
        }

        // Typed local of a different type: copy, then convert into the declared type.
        dsDATA converted;
        converted.type = nullptr;
        converted.StoreValue(val);

        if (!converted.Convert(local.pType, DSD_CONV_MATH))
        {
            const char *fromName = val.type ? val.type->GetDbgName() : "null";
            const char *toName = local.pType->GetDbgName();
            dsTSTRING<char> msg;
            sslERROR err(SSL_ERR_RUNTIME,
                         *dsSPrintf(&msg, "can't set local variable of type '%s' to '%s'", fromName, toName),
                         0);
            if (converted.type)
                converted.type->Destroy(&converted.storage);
            return err;
        }

        local.val.StoreValue(converted);
        if (converted.type)
            converted.type->Destroy(&converted.storage);
        return sslERROR();
    }

    dsTSTRING<char> msg;
    msg.pBuffer = nullptr;
    msg.UnsafeInit("local var index out of range", -1, 0);
    return sslERROR(SSL_ERR_RUNTIME, msg, 0);
}
