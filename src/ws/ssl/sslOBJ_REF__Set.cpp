#include "sslOBJ_REF.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsTSTRING.h"
#include "../ds/ds_boundary.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82524AC0 — sslOBJ_REF::Set. Returns sslERROR by value (sret): the decompiler swaps this/result
// (r3 = return slot sslERROR*, r4 = this sslOBJ_REF*). Copy `data` into a scratch value, explicitly
// convert it to an sslOBJ_REF, and retrieve it into this ref. `pSsl` is accepted for signature
// parity; the reversed body writes the result straight into this ref.
sslERROR sslOBJ_REF::Set(const dsDATA &data, sslSYSTEM *pSsl)
{
    (void)pSsl;

    dsDATA scratch;
    scratch.type = nullptr;
    scratch.StoreValue(data);

    if (scratch.Convert(dsDATA_TYPE_STORAGE<sslOBJ_REF>::dataType, DSD_CONV_EXPLICIT))
    {
        if (scratch.GetValue<sslOBJ_REF>(*this, DSD_CONV_RETRIEVE))
        {
            if (scratch.type)
                scratch.type->Destroy(&scratch.storage);
            return sslERROR();
        }

        const char *typeName = data.type ? data.type->GetDbgName() : "null";
        dsTSTRING<char> msg;
        sslERROR err(SSL_ERR_RUNTIME, *dsSPrintf(&msg, "can't get <%s> data to sslOBJ_REF", typeName), 0);
        if (scratch.type)
            scratch.type->Destroy(&scratch.storage);
        return err;
    }

    const char *typeName = data.type ? data.type->GetDbgName() : "null";
    dsTSTRING<char> msg;
    sslERROR err(SSL_ERR_RUNTIME, *dsSPrintf(&msg, "can't convert <%s> data to sslOBJ_REF", typeName), 0);
    if (scratch.type)
        scratch.type->Destroy(&scratch.storage);
    return err;
}
