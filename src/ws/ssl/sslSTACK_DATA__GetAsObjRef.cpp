#include "sslSTACK_DATA.h"
#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "sslCLASS.h"
#include "sslCLASS_REF.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsTSTRING.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82AF41B8 — sslSTACK_DATA::GetAsObjRef. Returns sslERROR by value (sret): the decompiler swaps
// this/result (r3 = return slot sslERROR*, r4 = this sslSTACK_DATA*).
//
// CAVEAT: the decompiler heavily overlaps one 16-byte stack slot, using it in turn as the fetched
// dsDATA, a scratch sslOBJ_REF, and the sslERROR return-copy source. It is untangled here into the
// named locals `data` / `ref`. The reference-payload fast path is gated on the raw discriminator
// value (type - 4 == 1, i.e. TYPE_SSLOBJECT_REF); reproduced faithfully.
sslERROR sslSTACK_DATA::GetAsObjRef(sslOBJECT *&obj, sslCLASS *&cls, sslSYSTEM *pSsl) const
{
    unsigned int kind = (unsigned int)(this->type - sslSTACK_DATA::TYPE_DS_DATA); // type - 4

    if (kind > 7)   // discriminator outside [TYPE_DS_DATA .. TYPE_DSDATA_MEMBER_REF]
    {
        dsTSTRING<char> msg;
        return sslERROR(SSL_ERR_GENERIC,
                        *dsSPrintf(&msg, "can't convert stack data %i to sslOBJ_REF", this->type),
                        0);
    }

    if (kind == 1)  // TYPE_SSLOBJECT_REF: payload already holds the object/class pair
    {
        obj = this->objRef.pObj;
        cls = this->objRef.pClass;
        return sslERROR();
    }

    // Any other reference kind: fetch the dsDATA form and convert it to an object ref.
    dsDATA data;
    data.type = nullptr;
    sslERROR fetchErr = GetAsDsData(data);
    if (fetchErr.id)
    {
        if (data.type)
            data.type->Destroy(&data.storage);
        return fetchErr;
    }

    sslOBJ_REF ref;
    sslERROR setErr = ref.Set(data, pSsl);
    if (data.type)
        data.type->Destroy(&data.storage);
    if (setErr.id)
        return setErr;

    obj = ref.pObject;
    cls = ref.pObject ? ref.pObject->refClass.pClass : nullptr;
    return sslERROR();
}
