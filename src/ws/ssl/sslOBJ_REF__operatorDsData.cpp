#include "sslOBJ_REF.h"
#include "../ds/dsDATA.h"

// sslOBJ_REF::operator dsDATA() const @ 0x825242D0 — conversion to a type-erased dsDATA.
// DEVIATION: decompiler mistyped both the sret return pointer and `this` as `sslOBJ_REF*`
// (it aliases the two hidden pointer args); the real signature per the PPC sret ABI is
// `dsDATA operator dsDATA() const` with the dsDATA* destination in r3 and `this` in r4.
// Body: zero the destination dsDATA's `type` field (marks it empty) before handing it to
// StoreValue<sslOBJ_REF>, which adopts the sslOBJ_REF dsDATA_TYPE descriptor and
// copy-constructs the value in place.
sslOBJ_REF::operator dsDATA() const
{
    dsDATA result;
    result.type = nullptr;
    result.StoreValue<sslOBJ_REF>(*this);
    return result;
}
