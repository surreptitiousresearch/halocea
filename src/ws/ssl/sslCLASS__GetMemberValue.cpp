/* ?GetMemberValue@sslCLASS@@QAA?AVsslERROR@@PAVsslOBJECT@@HAAVdsDATA@@@Z @0x82A39140 */
#include "sslCLASS.h"
#include "sslCLASS_ELEMENT.h"
#include "sslBASE_VAR.h"
#include "sslOBJECT.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsCONVERTION_TYPE.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary
// boundary — the process-wide descriptor for the null/empty value type.
struct NULL_TYPE;
template<class T> struct dsDATA_TYPE_STORAGE { static const dsDATA_TYPE *dataType; };

// 0x82A39140 — sslCLASS::GetMemberValue. Returns sslERROR by value (sret): the decompiler swaps
// this/result (r3 = return slot sslERROR*, r4 = this sslCLASS*) and mistypes pSelf as dsTSTRING<char>*.
//
// Element kinds 3, 4 and 5 are readable (var/const): the var object's virtual getter fills `val`, then
// the value is converted to the var's declared type. On any error (getter failure or conversion
// failure) `val` is reset to the null value and the error is returned. Every other element kind yields
// an "is not a var or const" runtime error. The range check uses the inclusive `idx > elements.nElem`
// bound the binary uses (reproduced faithfully).
//
// CAVEAT: the decompiler aliases the return sslERROR onto a scratch sslOBJECT slot, so its field reads
// print as `v42.dbgName.pBuffer` etc.; that access is really the error's `id` word (offset 0). The
// error test is therefore `err.id != 0`, untangled here.
sslERROR sslCLASS::GetMemberValue(sslOBJECT *pSelf, int idx, dsDATA &val)
{
    if (idx < 0 || idx > this->elements.nElem)
    {
        dsTSTRING<char> dbgName = pSelf->GetDbgName();
        dsTSTRING<char> msg;
        return sslERROR(SSL_ERR_RUNTIME,
                        *dsSPrintf(&msg, "index %i is not in range for object '%s'", idx, dbgName.CStr()),
                        0);
    }

    sslCLASS_ELEMENT &elem = this->elements[idx];
    if (elem.type == sslCLASS_ELEMENT::TYPE_CB_PROPERTY || elem.type == sslCLASS_ELEMENT::TYPE_STORED_MEMBER
     || elem.type == sslCLASS_ELEMENT::TYPE_CONST)
    {
        sslBASE_VAR *pVar = elem.pVar;
        sslERROR err = pVar->GetValue(pSelf, idx, val);

        if (!err.id)
        {
            const dsDATA_TYPE *declType = pVar->pType;
            if (declType != dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType
                && !val.Convert(declType, DSD_CONV_MATH))
            {
                const char *fromName = val.type ? val.type->GetDbgName() : "null";
                const char *toName = declType->GetDbgName();
                dsTSTRING<char> msg;
                err = sslERROR(SSL_ERR_RUNTIME,
                               *dsSPrintf(&msg, "can't convert get value from type <%s> to type <%s>", fromName, toName),
                               0);
            }
        }

        if (err.id)
        {
            // On any error, reset the out value to the null type (original:
            // dsDATA::StoreValue<NULL_TYPE>(val, &zeroNull)).
            const dsDATA_TYPE *nullType = dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType;
            dsDATA nullValue;
            nullValue.type = nullType;
            nullType->Construct(&nullValue.storage);
            val.StoreValue(nullValue);
            if (nullValue.type)
                nullValue.type->Destroy(&nullValue.storage);
        }
        return err;
    }

    // Not a readable (var/const) element.
    dsTSTRING<char> dbgName = pSelf->GetDbgName();
    dsTSTRING<char> msg;
    return sslERROR(SSL_ERR_RUNTIME,
                    *dsSPrintf(&msg, "element '%s.%s' is not a var or const", dbgName.CStr(), elem.name.CStr()),
                    0);
}
