#include "sslSTACK_DATA.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsTSTRING.h"
#include "../ds/ds_boundary.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82AF3A58 — sslSTACK_DATA::GetAsInt. Returns sslERROR by value (sret): the decompiler swaps
// this/result (r3 = return slot sslERROR*, r4 = this sslSTACK_DATA*). Fetch this value's dsDATA
// form; on a fetch error, propagate it; otherwise math-convert to int and read it out, or build a
// runtime "can't convert" error.
sslERROR sslSTACK_DATA::GetAsInt(int &val) const
{
    dsDATA data;
    data.type = nullptr;
    sslERROR fetchErr = GetAsDsData(data);

    if (fetchErr.id)   // GetAsDsData failed: propagate its error
    {
        if (data.type)
            data.type->Destroy(&data.storage);
        return fetchErr;
    }

    if (data.Convert(dsDATA_TYPE_STORAGE<int>::dataType, DSD_CONV_MATH))
    {
        data.GetValue<int>(val, DSD_CONV_RETRIEVE);
        if (data.type)
            data.type->Destroy(&data.storage);
        return sslERROR();
    }

    const char *typeName = data.type ? data.type->GetDbgName() : "null";
    dsTSTRING<char> msg;
    sslERROR err(SSL_ERR_RUNTIME, *dsSPrintf(&msg, "can't convert from <%s> to int", typeName), 0);
    if (data.type)
        data.type->Destroy(&data.storage);
    return err;
}
