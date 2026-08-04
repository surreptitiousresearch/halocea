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

// 0x82A38CA0 — sslCLASS::SetMemberValue. Returns sslERROR by value (sret): the decompiler swaps
// this/result (r3 = return slot sslERROR*, r4 = this sslCLASS*) and mistypes pSelf as dsTSTRING<char>*.
//
// Element kinds 3 and 4 are the settable variable kinds — the value is copied, converted to the var's
// declared type, then handed to the var object's virtual setter. Every other element kind yields an
// "is not a var" runtime error. The range check uses the inclusive `idx > elements.nElem` bound the
// binary uses (reproduced faithfully).
sslERROR sslCLASS::SetMemberValue(sslOBJECT *pSelf, int idx, const dsDATA &val)
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
    if (elem.type == sslCLASS_ELEMENT::TYPE_CB_PROPERTY || elem.type == sslCLASS_ELEMENT::TYPE_STORED_MEMBER)
    {
        // Settable variable: copy the incoming value, convert it to the var's declared type, then
        // dispatch the var object's virtual setter (which materialises the return sslERROR).
        dsDATA converted;
        converted.type = nullptr;
        converted.StoreValue(val);

        sslBASE_VAR *pVar = elem.pVar;
        const dsDATA_TYPE *declType = pVar->pType;

        if (declType == dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType
            || converted.Convert(declType, DSD_CONV_MATH))
        {
            // (The original also default-constructs an unused no-error sslERROR here; elided.)
            sslERROR setResult = pVar->SetValue(pSelf, idx, converted);
            if (converted.type)
                converted.type->Destroy(&converted.storage);
            return setResult;
        }

        // Conversion failed: report the type mismatch.
        const char *fromName = val.type ? val.type->GetDbgName() : "null";
        const char *toName = declType->GetDbgName();
        dsTSTRING<char> msg;
        sslERROR err(SSL_ERR_RUNTIME,
                     *dsSPrintf(&msg, "can't convert set value from type <%s> to type <%s>", fromName, toName),
                     0);
        if (converted.type)
            converted.type->Destroy(&converted.storage);
        return err;
    }

    // Not a variable element.
    dsTSTRING<char> dbgName = pSelf->GetDbgName();
    dsTSTRING<char> msg;
    return sslERROR(SSL_ERR_RUNTIME,
                    *dsSPrintf(&msg, "element '%s.%s' is not a var", dbgName.CStr(), elem.name.CStr()),
                    0);
}
